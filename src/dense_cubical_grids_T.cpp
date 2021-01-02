/* dense_cubical_grids.cpp

This file is part of CubicalRipser
Copyright 2017-2018 Takeki Sudo and Kazushi Ahara.
Modified by Shizuo Kaji

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License along
with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <initializer_list>

// switch V and T
#include "dense_cubical_grids.h"
#include "npy.hpp"

using namespace std;


DenseCubicalGrids::DenseCubicalGrids(Config& _config)  {
	config = &_config;
	threshold = config->threshold;
	config->tconstruction = true;
}

void DenseCubicalGrids::gridFromArray(vector<double>& arr, bool embedded){
//	cout << arr.size() << ", " << ax << ", " << ay << ", " << az << endl;
	if(arr.size() < ax*ay*az){
		cerr << "wrong file size." << endl;
		exit(-3);
	}
	uint64_t i = 0;
	if(embedded){ // dual complex
		if(az>1){
			dense3 = alloc3d(ax+4, ay+4, az+4);
			for (uint32_t z = 0; z < az + 4; ++z) {
				for (uint32_t y = 0; y < ay + 4; ++y) {
					for (uint32_t x = 0; x < ax + 4; ++x) {
						if (1 < x && x <= ax+1 && 1 < y && y <= ay+1 && 1 < z && z <= az+1) {
							dense3[x][y][z] = -arr[i++];
						}else if (0 == x || x == ax+3 || 0 == y || y == ay+3 || 0 == z || z == az+3) {
							dense3[x][y][z] = config->threshold;
						}else{
							dense3[x][y][z] = -config->threshold;
						}
	//					cout << x << "," << y << "," << z << ": " << dense3[x][y][z] << endl;
					}
				}
			}
			az = az + 2;
		}else{
			dense3 = alloc3d(ax+4, ay+4, az+2);
			for (uint32_t z = 0; z < az + 2; ++z) { // z-axis remains the same
				for (uint32_t y = 0; y < ay + 4; ++y) {
					for (uint32_t x = 0; x < ax + 4; ++x) {
						if (1 < x && x <= ax+1 && 1 < y && y <= ay+1 && 1<=z && z<= az) {
							dense3[x][y][z] = -arr[i++];
						}else if (0 == x || x == ax+3 || 0 == y || y == ay+3 || z==0 || z==az+1) {
							dense3[x][y][z] = config->threshold;
						}else{
							dense3[x][y][z] = -config->threshold;
						}
//						cout << x << "," << y << "," << z << ": " << dense3[x][y][z] << endl;
					}
				}
			}
		}
		ax = ax + 2;
		ay = ay + 2;				
	}else{
		dense3 = alloc3d(ax+2, ay+2, az+2);
		for (uint32_t z = 0; z < az + 2; ++z) {
			for (uint32_t y = 0; y < ay + 2; ++y) {
				for (uint32_t x = 0; x < ax + 2; ++x) {
					if (0 < x && x <= ax && 0 < y && y <= ay && 0 < z && z <= az) {
						dense3[x][y][z] = arr[i++];
					}else {
						dense3[x][y][z] = config->threshold;
					}
				}
			}
		}
	}
}

// from numpy array: note the order of axis
void DenseCubicalGrids::gridFromNpyArray(const double *arr, bool embedded){
	uint64_t i = 0;
	// load 
	if(embedded){
		if (az>1) {
			dense3 = alloc3d(ax + 4, ay + 4, az + 4);
			for (uint32_t x = 0; x < ax + 4; ++x) {
				for (uint32_t y = 0; y < ay + 4; ++y) {
					for (uint32_t z = 0; z < az + 4; ++z) {
						if (1 < x && x <= ax+1 && 1 < y && y <= ay+1 && 1 < z && z <= az+1) {
//							dense3[x][y][z] = -(*img.data(x-2, y-2, z-2));
							dense3[x][y][z] = -arr[i++];
//							cout << "ax : ay : az = " << ax << " : " << ay << " : " << az << endl;
						}else if (0 == x || x == ax+3 || 0 == y || y == ay+3 || 0 == z || z == az+3) {
							dense3[x][y][z] = config->threshold;
						}else{
							dense3[x][y][z] = -config->threshold;
						}
					}
				}
			}
			ax += 2;
			ay += 2;
			az += 2;
		}else{
			dense3 = alloc3d(ax + 4, ay + 4, az + 2);
			for (uint32_t x = 0; x < ax + 4; ++x) {
				for (uint32_t y = 0; y < ay + 4; ++y) {
					for (uint32_t z = 0; z < az + 2; ++z) {
						if (1 < x && x <= ax+1 && 1 < y && y <= ay+1 && 1<=z && z<= az) {
//							dense3[x][y][z] = -(*img.data(x-2, y-2));
							dense3[x][y][z] = -arr[i++];
						}else if (0 == x || x == ax+3 || 0 == y || y == ay+3 || z==0 || z==az+1) {
							dense3[x][y][z] = config->threshold;
						}else{
							dense3[x][y][z] = -config->threshold;
						}
					}
				}
			}
			ax += 2;
			ay += 2;
		}
	}else{
		if (az>1) {
			dense3 = alloc3d(ax + 2, ay + 2, az + 2);
			for (uint32_t x = 0; x < ax + 2; ++x) {
				for (uint32_t y = 0; y < ay + 2; ++y) {
					for (uint32_t z = 0; z < az + 2; ++z) {
						if (0 < x && x <= ax && 0 < y && y <= ay && 0 < z && z <= az) {
							dense3[x][y][z] = arr[i++];
						}
						else { // fill the boundary with the threashold value
							dense3[x][y][z] = config->threshold;
						}
					}
				}
			}
		}else if(ay>1) {
			dense3 = alloc3d(ax + 2, ay + 2, az + 2);
			for (uint32_t x = 0; x < ax + 2; ++x) {
				for (uint32_t y = 0; y < ay + 2; ++y) {
					for (uint32_t z = 0; z < az + 2; ++z) {
						if (0 < x && x <= ax && 0 < y && y <= ay && 0 < z && z <= az) {
							dense3[x][y][z] = arr[i++];
						}
						else { // fill the boundary with the threashold value
							dense3[x][y][z] = config->threshold;
						}
					}
				}
			}
		}else{
			dense3 = alloc3d(ax + 2, ay + 2, az + 2);
			for (uint32_t x = 0; x < ax + 2; ++x) {
				for (uint32_t y = 0; y < ay + 2; ++y) {
					for (uint32_t z = 0; z < az + 2; ++z) {
						if (0 < x && x <= ax && 0 < y && y <= ay && 0 < z && z <= az) {
							dense3[x][y][z] = arr[i++];
						}
						else { // fill the boundary with the threashold value
							dense3[x][y][z] = config->threshold;
						}
					}
				}
			}
		}
	}
}


// different for V and T
// read from file
void DenseCubicalGrids::loadImage(bool embedded){
	// read file
	cout << "Reading " << config->filename << endl;
	switch(config->format){
		case DIPHA:
		{
			ifstream fin( config->filename, ios::in | ios::binary );

			int64_t d;
			fin.read( ( char * ) &d, sizeof( int64_t ) ); // magic number
			assert(d == 8067171840);
			fin.read( ( char * ) &d, sizeof( int64_t ) ); // type number
			assert(d == 1);
			fin.read( ( char * ) &d, sizeof( int64_t ) ); //data num
			fin.read( ( char * ) &d, sizeof( int64_t ) ); // dim 
			dim = d;
			assert(dim < 4);
			fin.read( ( char * ) &d, sizeof( int64_t ) );
			ax = d;
			if (dim>1) {
				fin.read( ( char * ) &d, sizeof( int64_t ) );
				ay = d;
			}else{
				ay = 1;
			}
			if (dim>2) {
				fin.read((char *)&d, sizeof(int64_t));
				az = d;
			}else {
				az = 1;
			}
			cout << "ax : ay : az = " << ax << " : " << ay << " : " << az << endl;
			img_x = ax;
			img_y = ay;
			img_z = az;
			double dou;
			vector<double> arr;
			arr.reserve(ax*ay*az);
			while (!fin.eof()){
				fin.read((char *)&dou, sizeof(double));
				arr.push_back(dou);
			}
			fin.close();
			gridFromArray(arr, embedded);
			break;
		}

		case PERSEUS:
		{
			ifstream reading_file; 
			reading_file.open(config->filename.c_str(), ios::in); 

			string reading_line_buffer; 
			getline(reading_file, reading_line_buffer); 
			dim = atoi(reading_line_buffer.c_str());
			assert(dim < 4);
			getline(reading_file, reading_line_buffer);
			ax = atoi(reading_line_buffer.c_str()); 
			if (dim>1) {
				getline(reading_file, reading_line_buffer); 
				ay = atoi(reading_line_buffer.c_str()); 
			}else {
				ay = 1;
			}
			if (dim>2) {
				getline(reading_file, reading_line_buffer);
				az = atoi(reading_line_buffer.c_str());
			}else {
				az = 1;
			}
			cout << "ax : ay : az = " << ax << " : " << ay << " : " << az << endl;
			img_x = ax;
			img_y = ay;
			img_z = az;
			vector<double> arr;
			arr.reserve(ax*ay*az);
			while(!reading_file.eof()){
				getline(reading_file, reading_line_buffer);
				double dou = atof(reading_line_buffer.c_str());
				if (dou != -1) {
					arr.push_back(dou);
				}else{
					arr.push_back(config->threshold);
				}
			}
			reading_file.close();
			gridFromArray(arr, embedded);
			break;
		}

		case CSV:
		{
			dim = 2;
			vector<double> arr;
			ifstream reading_file; 
			reading_file.open(config->filename.c_str(), ios::in); 
			string line;
			ay = 0;
    		while (getline(reading_file, line)) {
				istringstream stream(line);
				string field;
				ax = 0;
				while (getline(stream, field, ',')) {
					arr.push_back(stod(field));
					ax++;
				}
				ay++;
			}
			az = 1;
			cout << "ax : ay : az = " << ax << " : " << ay << " : " << az << endl;
			img_x = ax;
			img_y = ay;
			img_z = az;
			gridFromArray(arr, embedded);
			break;
		}

		case NUMPY:
		{
			vector<unsigned long> shape;
			vector<double> arr;
			try{
				npy::LoadArrayFromNumpy(config->filename.c_str(), shape, arr);
			} catch (...) {
				cerr << "The data type of an numpy array should be numpy.float64." << endl;
				exit(-2);
			}
			if(shape.size() > 3){
				cerr << "Input array should be 1,2 or 3 dimensional " << endl;
				exit(-1);
			}
			dim = shape.size();
			ax = shape[0];
			if (dim>1) {
				ay = shape[1];
			}else {
				ay = 1;
			}
			if (dim>2) {
				az = shape[2];
			}else {
				az = 1;
			}
			cout << "ax : ay : az = " << ax << " : " << ay << " : " << az << endl;
			img_x = ax;
			img_y = ay;
			img_z = az;
			gridFromNpyArray(&arr[0], embedded);
			break;
		}
	}	
    // T-construction
    if(true){
        if(az>1) az++;
        ax++;
        ay++;
    }
    //
	axy = ax * ay;
	ayz = ay * az;
	axyz = ax * ay * az;
//	cout << ax << "," << ay << "," << az << endl;
}

// return filtlation value for a cube
double DenseCubicalGrids::getBirth(uint32_t cx, uint32_t cy, uint32_t cz){
	return min({ dense3[cx][cy][cz], dense3[cx+1][cy][cz],
				dense3[cx+1][cy+1][cz], dense3[cx][cy+1][cz],
				dense3[cx][cy][cz+1], dense3[cx+1][cy][cz+1],
				dense3[cx+1][cy+1][cz+1], dense3[cx][cy+1][cz+1] });
}
double DenseCubicalGrids::getBirth(uint32_t cx, uint32_t cy, uint32_t cz, uint8_t cm, uint8_t dim) {
	// beware of the shift due to the boundary
	switch (dim) {
		case 3:
			return dense3[cx+1][cy+1][cz+1];
		case 2:
			switch (cm) {
				case 0: // fix z
					return min(dense3[cx+1][cy+1][cz], dense3[cx + 1][cy + 1][cz + 1]);
				case 1: // fix y
					return min(dense3[cx+1][cy][cz+1], dense3[cx + 1][cy + 1][cz + 1]);
				case 2: // fix x
					return min(dense3[cx][cy+1][cz+1], dense3[cx + 1][cy + 1][cz + 1]);
				break;
			}
		case 1:
			switch (cm) {
				case 0: // x,x+1
					return min({ dense3[cx+1][cy+1][cz+1], dense3[cx+1][cy+1][cz],
						dense3[cx+1][cy][cz+1], dense3[cx+1][cy][cz] });
				case 1: // y,y+1
					return min({ dense3[cx+1][cy+1][cz+1], dense3[cx][cy+1][cz+1],
						dense3[cx+1][cy+1][cz], dense3[cx][cy+1][cz] });
				case 2: // z,z+1
					return min({ dense3[cx+1][cy+1][cz+1], dense3[cx][cy+1][cz+1],
						dense3[cx+1][cy][cz+1], dense3[cx][cy][cz+1] });
				break;
			}
		case 0:
			return min({ dense3[cx][cy][cz], dense3[cx+1][cy][cz],
				dense3[cx+1][cy+1][cz], dense3[cx][cy+1][cz],
				dense3[cx][cy][cz+1], dense3[cx+1][cy][cz+1],
				dense3[cx+1][cy+1][cz+1], dense3[cx][cy+1][cz+1] });
		}
	return threshold; // dim > 3
}

// (x,y,z) of the voxel which defines the birthtime of the cube
vector<uint32_t> DenseCubicalGrids::ParentVoxel(uint8_t _dim, Cube &c){
	uint32_t cx = c.x();
	uint32_t cy = c.y();
	uint32_t cz = c.z();
	if(c.birth == dense3[cx+1][cy+1][cz+1]){
		return {cx,cy,cz};
	}else if(c.birth == dense3[cx+0][cy+1][cz+1]){			// T-construction
		return { cx - 1,cy,cz };
	}else if(c.birth == dense3[cx+0][cy+0][cz+1]){
		return { cx - 1,cy-1,cz };
	}else if(c.birth == dense3[cx+0][cy+0][cz+0]){
		return { cx - 1,cy-1,cz-1 };
	}else if(c.birth == dense3[cx+0][cy+1][cz+0]){
		return { cx - 1,cy,cz-1 };
	}else if(c.birth == dense3[cx+1][cy+0][cz+1]){
		return { cx,cy-1,cz};
	}else if(c.birth == dense3[cx+1][cy+0][cz+0]){
		return { cx,cy-1,cz-1};
	}else if(c.birth == dense3[cx+1][cy+1][cz+0]){
		return { cx,cy,cz-1};
	}else{
		cerr << "parent voxel not found!" << endl;
		return { 0,0,0 };
	}
}
	
DenseCubicalGrids::~DenseCubicalGrids(){
	free(dense3[0][0]);
	free(dense3[0]);
	free(dense3);
}

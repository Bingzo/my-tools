#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

const double PI = 3.1415926535897932384626433832795;

enum direct{A, B, C, AB, BC, AC, ABC};
typedef struct{
	double x;
	double y;
	double z;
}coord;

const double deltaKp = 0.0001;
class pairPoint{
public:
	pairPoint(coord p[]);
	coord points[2];
	direct dir;
};
pairPoint::pairPoint(coord p[]){
	for(int i = 0; i < 2; i++){
		points[i].x = p[i].x;
		points[i].y = p[i].y;
		points[i].z = p[i].z;
	}
	if(abs(p[1].x - p[0].x) > 0.001 && abs(p[1].y - p[0].y) > 0.001 && abs(p[1].z - p[0].z) > 0.001){
		dir = ABC;
	}
	else if(abs(p[1].x - p[0].x) > 0.001 && abs(p[1].z - p[0].z) > 0.001){
		dir = AC;
	}
	else if(abs(p[1].y - p[0].y) > 0.001 && abs(p[1].z - p[0].z) > 0.001){
		dir = BC;
	}
	else if(abs(p[1].x - p[0].x) > 0.001 && abs(p[1].y - p[0].y) > 0.001){
		dir = AB;
	}
	else if(abs(p[1].z - p[0].z) > 0.001){
		dir = C;
	}
	else if(abs(p[1].y - p[0].y) > 0.001){
		dir = B;
	}
	else if(abs(p[1].x - p[0].x) > 0.001){
		dir = A;
	}
}

int main(int argc, char *argv[]){
	ifstream ifs;
	if(argc > 1){
		ifs.open(argv[1], ios_base::in);
	}
	else{
		ifs.open("Kpoints.txt", ios_base::in);
	}
	if(ifs.bad()){
		cerr << "File can't be opened." << endl;
		return -1;
	}

	size_t found;
	string str;
	int nPoints;
	vector<pairPoint> vPoints;
	double abc[3];
	int fP;
	int ret = 0;
	while(!ifs.eof() && 0 == ret){
		getline(ifs, str);
		found = str.find("k-points");
		if(found != string::npos){
			ifs >> nPoints; getline(ifs, str);
		}
		getline(ifs, str);
		found = str.find("Line-mode");
		getline(ifs, str);
		if(found != string::npos){
			coord p[2];

			while(0 == ret){
				fP = ifs.tellg();
				getline(ifs, str);
				size_t fd = str.find("a-b-c");
				if(fd != string::npos){
					char c[2];
					ifs >> c[0] >> c[1] >> abc[0]; getline(ifs, str);
					ifs >> c[0] >> c[1] >> abc[1]; getline(ifs, str);
					ifs >> c[0] >> c[1] >> abc[2]; getline(ifs, str);
					getline(ifs, str);
					ret = 1;
					break;
				}
				if(str != ""){
					ifs.seekg(fP, ios::beg);
					for(int i = 0; i < 2; i++){
						ifs >> p[i].x >> p[i].y >> p[i].z; getline(ifs, str);
					}
					pairPoint pPt(p);
					vPoints.push_back(pPt);
				}
			}
		}
	}

	ofstream ofs("out.txt", ios_base::trunc);
	vector<pairPoint>::iterator it;
	double lengthAxis = 0.0;
	//vector<int> vlengthAxis;
	double incX = 0.0;
	double incY = 0.0;
	double incZ = 0.0;
	for(it = vPoints.begin(); it != vPoints.end(); it ++){
		switch((*it).dir){
		case A:
			incX = ((*it).points[1].x - (*it).points[0].x) / 10;
			for(int i = 0; i < 10; i ++){
				lengthAxis += abs(2*PI*incX/abc[0]);
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case B:
			incY = ((*it).points[1].y - (*it).points[0].y) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += abs(2*PI*incY/abc[1]);
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case C:
			incZ = ((*it).points[1].z - (*it).points[0].z) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += abs(2*PI*incZ/abc[2]);
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case AB:
			incX = ((*it).points[1].x - (*it).points[0].x) / 10.0;
			incY = ((*it).points[1].y - (*it).points[0].y) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += sqrt(pow(2*PI*incX/abc[0], 2.0) + pow(2*PI*incY/abc[1], 2.0));
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case BC:
			incY = ((*it).points[1].y - (*it).points[0].y) / 10.0;
			incZ = ((*it).points[1].z - (*it).points[0].z) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += sqrt(pow(2*PI*incY/abc[1], 2.0) + pow(2*PI*incZ/abc[2], 2.0));
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case AC:
			incX = ((*it).points[1].x - (*it).points[0].x) / 10.0;
			incZ = ((*it).points[1].z - (*it).points[0].z) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += sqrt(pow(2*PI*incX/abc[0], 2.0) + pow(2*PI*incZ/abc[2], 2.0));
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		case ABC:
			incX = ((*it).points[1].x - (*it).points[0].x) / 10.0;
			incY = ((*it).points[1].y - (*it).points[0].y) / 10.0;
			incZ = ((*it).points[1].z - (*it).points[0].z) / 10.0;
			for(int i = 0; i < 10; i ++){
				lengthAxis += sqrt(pow(2*PI*incX/abc[0], 2.0) + pow(2*PI*incY/abc[1], 2.0) + pow(2*PI*incZ/abc[2], 2.0));
				//vlengthAxis.push_back(lengthAxis);
				ofs << lengthAxis << endl;
			}
			break;
		default:
			break;
		}
	}

	ifs.close();
	ofs.close();
	return 0;
}
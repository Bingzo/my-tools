#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

typedef struct removeDelimiter{
	bool operator()(char c){
		return (c == '\r' || c == '\t' || c == ' ' || c == '\n');
	}
}removeDelimiter;

typedef struct coor{
	double x;
	double y;
	double z;
}coord;

typedef struct dump{
	int timestep;
	int nAtoms;
	vector<coord> vCoord;
}dump;

int main(int argc, char *argv[]){
	ifstream ifs;
	if(argc > 1){
		ifs.open(argv[1], ios_base::in);
	}
	else{
		//ifs.open("np.300.atom", ios_base::in);
		cerr << "Input file not specified." << endl;
		return -1;
	}
	if(!ifs){
		cerr << "Failed to open the input file." << endl;
		return -1;
	}

	string str;
	dump dp;
	vector<dump> vdp;
	while(!ifs.eof()){
		getline(ifs, str);
		str.erase(remove_if(str.begin(), str.end(), removeDelimiter()), str.end());
		if(str != ""){
			ifs >> dp.timestep; getline(ifs, str);
			getline(ifs, str);
			ifs >> dp.nAtoms; getline(ifs, str);
			for(int i = 0; i < 5; i ++){
				getline(ifs, str);
			}
			int id, type;
			coord cd;
			dp.vCoord.clear();
			for(int i = 0; i < dp.nAtoms; i ++){
				ifs >> id >> type >> cd.x >> cd.y >> cd.z; getline(ifs, str);
				dp.vCoord.push_back(cd);
			}
			vdp.push_back(dp);
		}
	}
	ifs.close();

	double sum = 0.0;
	vector<dump>::iterator it;
	int nAtoms = vdp[0].nAtoms;
	double sqr = 0.0;
	double r = 0.0;
	for(int i = 0; i < nAtoms; i ++){
		for(int j = 0; j < nAtoms; j ++){
			if(i < j){
				int cnt = 0;
                sqr = 0.0;
                r = 0.0;
				for(it = vdp.begin(); it != vdp.end(); it ++){
					++ cnt;
					double r2 = pow((*it).vCoord[i].x - (*it).vCoord[j].x, 2.0) \
					            + pow((*it).vCoord[i].y - (*it).vCoord[j].y, 2.0) \
						        + pow((*it).vCoord[i].z - (*it).vCoord[j].z, 2.0);
                    sqr += r2;
                    if(r2 > 0.0001){
					    r += pow(r2, 0.5);
                    }
				}
				sqr /= cnt;
				r /= cnt;
				if(sqr - pow(r, 2.0) > 0.0001){
                    sum += pow(sqr - pow(r, 2.0), 0.5) / r;
                }
			}
		}
	}
	double sigma;
	sigma = (2.0 * sum) / (nAtoms * (nAtoms - 1));
	cout << "sigma: " << sigma << endl;
	
	return 0;
}

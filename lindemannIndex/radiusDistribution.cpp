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
    vector<int> vLat;
}dump;

typedef struct eTime{
    int data[10][6];
}eTime;

const double DELTA = 1e-6;
const double PI = 3.1415926535898;

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
			int id, type, ilat;
			coord cd;
			dp.vCoord.clear();
			for(int i = 0; i < dp.nAtoms; i ++){
				ifs >> id >> type >> cd.x >> cd.y >> cd.z >> ilat; getline(ifs, str);
				dp.vCoord.push_back(cd);
                dp.vLat.push_back(ilat);
			}
			vdp.push_back(dp);
		}
	}
	ifs.close();


	vector<dump>::iterator it;
	int nAtoms = vdp[0].nAtoms;

    typedef struct sphere{
        coord c;
        double r;
    }sphere;

    ofstream ofs("rdis.txt", ios_base::trunc);
    ofs << "timestep\t\tr\t\t\t\t\tn\t\t1\t\t2\t\t3\t\t4\t\t5" << endl;
    ofs << "\t\t\t\t \t\t\t\t\t\t \tfcc\t\thcp\t\tbcc\t\tico\t\tunknown " << endl;

    for(it = vdp.begin(); it != vdp.end(); it ++){
        ofs << (*it).timestep << endl;
        sphere s;
        double max[3] = {(*it).vCoord[0].x, (*it).vCoord[0].y, (*it).vCoord[0].z};
        double min[3] = {(*it).vCoord[0].x, (*it).vCoord[0].y, (*it).vCoord[0].z};
        for(int i = 1; i < nAtoms; i ++){
           if(max[0] < (*it).vCoord[i].x + DELTA){
                max[0] = (*it).vCoord[i].x;
           }
           if(max[1] < (*it).vCoord[i].y + DELTA){
                max[1] = (*it).vCoord[i].y;
           }
           if(max[2] < (*it).vCoord[i].z + DELTA){
                max[2] = (*it).vCoord[i].z;
           }
           if(min[0] + DELTA > (*it).vCoord[i].x){
                min[0] = (*it).vCoord[i].x;
           }
           if(min[1] + DELTA > (*it).vCoord[i].y){
                min[1] = (*it).vCoord[i].y;
           }
           if(min[2] + DELTA > (*it).vCoord[i].z){
                min[2] = (*it).vCoord[i].z;
           }
        }
        for(int i = 0; i < 3; i ++){
            max[i] += DELTA;
        }
        s.c.x = 0.5 * (max[0] + min[0]);
        s.c.y = 0.5 * (max[1] + min[1]);
        s.c.z = 0.5 * (max[2] + min[2]);
        s.r = 0.5 * sqrt((max[0] - min[0])*(max[0] - min[0]) \
                + (max[1] - min[1])*(max[1] - min[1]) \
                + (max[2] - min[2])*(max[2] - min[2]));
        double r[10];
        double R3= pow(s.r, 3.0);
        for(int i = 0; i < 10; i ++){
            r[i] = pow(0.1*(i+1)*R3, 1.0/3);
        }
        int cnt;
        int latcnt[5];
        double r2[11];
        r2[0] = 0.0;
        for(int i = 0; i < 10; i ++){
            r2[i+1] = r[i]*r[i];
        }
        for(int i = 0; i < 10; i ++){
            cnt = 0;
            for(int k = 0; k < 5; k ++){
                latcnt[k] = 0;
            }
            for(int j = 0; j < nAtoms; j++){
                double x = (*it).vCoord[j].x - s.c.x;
                double y = (*it).vCoord[j].y - s.c.y;
                double z = (*it).vCoord[j].z - s.c.z;
                double dR2 = x*x + y*y + z*z;  
                if(dR2 > r2[i] && dR2 < r2[i+1]){
                    ++ cnt;
                    ++ latcnt[(*it).vLat[j] - 1];
                }
            }
            ofs << "\t\t\t" << i << '(' << sqrt(r2[i]) << '~' << sqrt(r2[i+1]) << ')';
            if(i == 0) ofs << "   \t";
            ofs << " \t\t" \
                << cnt << "\t\t" \
                << latcnt[0] << "\t\t" \
                << latcnt[1] << "\t\t" \
                << latcnt[2] << "\t\t" \
                << latcnt[3] << "\t\t" \
                << latcnt[4] << "\t\t" << endl;
        }
        ofs << endl;
    }
    ofs.close();

//////////////////////////////////
    ifs.open("rdis.txt", ios_base::in);
    ofs.open("rdis_avg.txt", ios_base::trunc);
    for(int i = 0; i < 2; i ++){
        getline(ifs, str);
        ofs << str << endl;
    }
    eTime et;
    vector<eTime> vet;
    while(!ifs.eof()){
        getline(ifs, str);
        for(int i = 0; i < 10; i ++){
            ifs >> str;
            for(int j = 0; j < 6; j++){
                ifs >> et.data[i][j];
            }
            getline(ifs, str);
        }
        vet.push_back(et);
        getline(ifs, str);
    }
    vector<eTime>::iterator iet;
    double sum[10][6];
    for(int i = 0; i < 10; i ++){
        for(int j = 0; j < 6; j ++){
            sum[i][j] = 0.0;
        }
    }
    for(int i = 0; i < 10; i ++){
        for(int j = 0; j < 6; j ++){
            for(iet = vet.begin(); iet != vet.end(); iet ++){
                sum[i][j] += (*iet).data[i][j];
            }
       }
    }
    for(int i = 0; i < 10; i ++){
        ofs << "\t\t\t\t" << i << "\t\t\t\t\t";
        for(int j = 0; j < 6; j ++){
            sum[i][j] /= vet.size();
            ofs << int(sum[i][j]+0.5) << "\t\t";
        }
        ofs << endl;
    }

    ifs.close();
    ofs.close();
	
	return 0;
}

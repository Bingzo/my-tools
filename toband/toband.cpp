/**
 * @file toband.cpp
 * @Synopsis  
 * @author Bing Zhou   zhoubinghust@126.com
 * @version 1.0.0
 * @date 2014-04-30
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

typedef struct kpInfo{
    int ndir;
    int nn;
}kpInfo;

typedef struct coord{
    double x;
    double y;
    double z;
}coord;

int main(int argc, char *argv[]){
    ifstream ifs;
    if(argc > 1){
        ifs.open(argv[1], ios_base::in);
    }
    else{
        ifs.open("BANDCTR", ios_base::in);
    }
    if(ifs.bad()){
        cerr << "Failed to open the input file." << endl;
        return -1;
    }

    string str;
    kpInfo kp;
    double len;

    getline(ifs, str);
    ifs >> kp.ndir; getline(ifs, str);
    ifs >> kp.nn; getline(ifs, str);

    ofstream ofs("KPOINTS", ios_base::trunc);
    if(ofs.bad()){
        cerr << "Failed to write the output file." << endl;
        return -1;
    }
    ofs << "k-points along high symmetry lines" << endl;
    ofs << kp.ndir * kp.nn << endl;
    ofs << "Line-mode" << endl;
    ofs << "rec" << endl;
    ofs << endl;

    coord c[2];
    for(int i = 0; i < kp.ndir; i ++){
        ifs >> str >> len; getline(ifs, str);
        for(int j = 0; j < 2; j ++){
            ifs >> c[j].x >> c[j].y >> c[j].z;
            getline(ifs, str);
        }
        coord cTemp;
        cTemp.x = (c[1].x>c[0].x? 1:-1)* \
                  (c[1].x-c[0].x)*(c[1].x-c[0].x)/(len*kp.nn);
        cTemp.y = (c[1].y>c[0].y? 1:-1)* \
                  (c[1].y-c[0].y)*(c[1].y-c[0].y)/(len*kp.nn);
        cTemp.z = (c[1].z>c[0].z? 1:-1)* \
                  (c[1].z-c[0].z)*(c[1].z-c[0].z)/(len*kp.nn);
        for(int j = 1; j < kp.nn+1; j ++){
            ofs << c[0].x + (j-1)*cTemp.x << ' ' \
                << c[0].y + (j-1)*cTemp.y << ' ' \
                << c[0].z + (j-1)*cTemp.z << endl;
            ofs << c[0].x + j*cTemp.x << ' ' \
                << c[0].y + j*cTemp.y << ' ' \
                << c[0].z + j*cTemp.z << endl;
            ofs << endl;
        }
        ofs << endl;
    }

    ifs.close();
    ofs.close();

    return 0;
}


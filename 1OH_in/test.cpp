/**
 * @file test.cpp
 * @Synopsis  for TianTong
 * @author Bing Zhou   zhoubinghust@126.com
 * @version 1.0.0
 * @date 2014-07-09
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
using namespace std;

typedef struct{
    double z;
    vector<double> ene;
    vector<double> dos;
}ZDos;

bool dequal(double di, double dj) {
    return (abs(di - dj) < 1.0e-2? true: false);
}

bool zless(ZDos &zi, ZDos &zj) {
    return (zi.z < zj.z? true: false);
}

int main(){
    int natoms;
    //cout << "number of atoms: ";
    //cin >> natoms;
    //cout << endl;
    natoms = 83;

    vector<double> z0;

    string str;
    double dtemp;

    ifstream ifsPos( "CONTCAR", ios_base::in );
    double zscale;
    for(int i = 0; i < 4; i ++){
        getline(ifsPos, str);
    }

    ifsPos >> dtemp >> dtemp >> zscale; getline(ifsPos, str);

    for(int i = 0; i < 4; i ++){
        getline(ifsPos, str);
    }

    for(int i = 0; i < natoms; i ++){
        double x, y, z;
        ifsPos >> x >> y >> z; getline(ifsPos, str);
        z0.push_back(z);
    }
    ifsPos.close();

    ifstream ifsDos( "DOSCAR", ios_base::in );
    for(int i = 0; i < 5; i ++){
        getline(ifsDos, str);
    }

    int ndos;
    ifsDos >> dtemp >> dtemp >> ndos; getline(ifsDos, str);

    vector<double> vtemp(ndos);
    ZDos zdtemp = {0.0, vtemp, vtemp};
    vector<ZDos> zdos(natoms, zdtemp);

    //for(int i = 0; i < natoms; i ++){
    //    zdos[i].ene.resize(ndos);
    //    zdos[i].dos.resize(ndos);
    //}
    for(int i = 0; i < ndos + 1; i ++){
        getline(ifsDos, str);
    }
    for(int i = 0; i < natoms; i ++){
        zdos[i].z = z0[i];
        for(int j = 0; j < ndos; j ++){
            ifsDos >> zdos[i].ene[j] >> dtemp >> dtemp >> dtemp >> zdos[i].dos[j]; getline(ifsDos, str);
        }
        getline(ifsDos, str);
    }
    ifsDos.close();

    sort(z0.begin(), z0.end());
    for(int i = 0; i < z0.size(); i ++)
        cout << z0[i] << ' ';
    cout << endl;
    cout << z0.size() << endl;
    vector<double>::iterator it;
    it = unique(z0.begin(), z0.end(), dequal);
    z0.resize(distance(z0.begin(), it));
    for(int i = 0; i < z0.size(); i ++)
        cout << z0[i] << ' ';
    cout << endl;
    cout << z0.size() << endl;

    vector<ZDos> unizdos(z0.size(), zdtemp);

    sort(zdos.begin(), zdos.end(), zless);

    vector<ZDos>::iterator itzd = zdos.begin();
    vector<double> dos(ndos, 0.0);
    for(int i = 0; i < z0.size(); ){
        //cout << i  << endl;
        unizdos[i].z = z0[i];
        if(dequal((*itzd).z, z0[i])){
            for(int j = 0; j < ndos; j ++){
                dos[j] += (*itzd).dos[j];        
            }
            ++ itzd;
        }
        else{
            unizdos[i].ene = (*(itzd-1)).ene;
            unizdos[i].dos = dos;
            for(int j = 0; j < ndos; j ++){
                dos[j] = 0.0;
            }
            ++ i;
        }
    }

    ofstream ofs("out.txt", ios_base::trunc);
    ofs.setf(ios::left);
    for(int i = 0; i < unizdos.size(); i ++){
        for(int j = 0; j < unizdos[i].dos.size(); j ++){
            if(abs(unizdos[i].ene[j] + 2.28193488) < 3.0){
                ofs << setw(8) << unizdos[i].z << ' ' << setw(8) << unizdos[i].ene[j] + 2.28193488 << ' ' << setw(8) << unizdos[i].dos[j] << endl; 
            }
        }
    }
    ofs.close();

    return 0;
}

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
using namespace std;

typedef struct{
    float ene[2];
    int n;
    float feim_ene;
}Dos;
typedef struct{
    float ene;
    float zdos;
}PDos;
//typedef struct{
//    float z;
//    float ene;
//    float zdos;
//}ZDos;

int main(){
    ifstream ifsDos( "DOSCAR", ios_base::in );
    ifstream ifsPos( "CONTCAR", ios_base::in );

    int ini_atom;
    int lst_atom;
    int lower_ene;
    int upper_ene;

    cout << "initial atom(int): "; 
    cin >> ini_atom;
    cout << endl;

    cout << "last atom(int): ";
    cin >> lst_atom;
    cout << endl;

    cout << "lower energy(int): ";
    cin >> lower_ene;
    cout << endl;

    cout << "upper energy(int): ";
    cin >> upper_ene;
    cout << endl;

    string str;
    int natoms = lst_atom - ini_atom + 1;
    float yz_scale[2];

    for(int i = 0; i < 3; i ++){
        getline(ifsPos, str);
    }

    for(int i = 0; i < 2; i ++){
        float tmp;
        ifsPos >> tmp >> yz_scale[0] >> tmp; getline(ifsPos, str);
        ifsPos >> tmp >> tmp >> yz_scale[1]; getline(ifsPos, str);
    }

    //vector<float> y;
    vector<float> z;
    for(int i = 0; i < 4 + (ini_atom - 1); i ++){
        getline(ifsPos, str);
    }

    for(int i = 0; i < natoms; i ++){
        float x, y, z;
        ifsPos >> x >> y >> z; getline(ifsPos, str);
        //y.push_back(y);
        z.push_back(z * yz_scale);
    }
    ifsPos.close();

    Dos dos;
    vector<PDos> pdos;
    int pdos_size;
    for(int i = 0; i < 5; i ++){
        getline(ifsDos, str);
    }
    ifsDos >> dos.ene[1] >> dos.ene[0] >> dos.n >> dos.feim_ene; getline(ifsDos, str);
    dos.ene[0] += dos.feim_ene;
    dos.ene[1] += dos.feim_ene;
    d_ene = (dos.ene[1] - dos.ene[0]) / (dos.n-1);
    int range[2];
    range[0] = int((lower_ene - dos.ene[0]) / d_ene);
    range[1] = int((upper_ene - dos.ene[0]) / d_ene);
    for(int i = 0; i < dos.n + 1; i ++){
        getline(ifsDos, str);
    }
    for(int i = 0; i < natoms; i ++){
        PDos pd;
        float tmp;
        for(int j = 0; j < range[1]; j ++){
            if(j < range[0]){
                getline(ifsDos, str);
            }
            else{
                ifsDos >> pd.e >> tmp >> tmp >> tmp >> pd.zdos; getline(ifsDos, str);
                pdos.push_back(pd);
            }
        }
    }
    pdos_size = pdos.size / natoms;
    ifsDos.close();

    vector<float> zn;
    float z_min, z_max;
    z_min = *(min_element(z.begin(), z.end()));
    z_max = *(max_element(z.begin(), z.end()));
    for(float f = z_min; f < z_max + 0.05; f += 0.05){
        zn.push_back(f);
    }
    
    //vector<ZDos> zdos;
    vector<float> zndos(zn.size, 0.0);
    for(int i = 0; i < natoms; i ++){
        for(int j = 0; j < zn.size(); j ++){
            if( abs(zn[j] - z[k]) < 0.025+0.001 ){
                 
            }
        }
    }

    return 0;
}

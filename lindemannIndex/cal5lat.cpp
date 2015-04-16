#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

typedef struct removeDelimiter{
    bool operator()(char c){
        return (c == '\r' || c == '\t' || c == ' ' || c == '\n');
    }
}removeDelimiter;

typedef struct dump{
    int timestep;
    int cnt[5];
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

    ofstream ofs("lat.txt", ios_base::trunc);
    ofs << "timestep\t\t1      2      3      4     5     " << endl;
    ofs << "       \t\t\tfcc    hcp    bcc    ico  unknown " << endl;

    string str;
    dump dp;
    int nAtoms;
    while(!ifs.eof()){
        getline(ifs, str);
        str.erase(remove_if(str.begin(), str.end(), removeDelimiter()), str.end());
        if(str != ""){
            for(int i = 0; i < 5; i ++){
                dp.cnt[i] = 0;
            }
            ifs >> dp.timestep; getline(ifs, str);
            getline(ifs, str);
            ifs >> nAtoms; getline(ifs, str);
            for(int i = 0; i < 5; i ++){
               getline(ifs, str); 
            }
            int id, type, ilat;
            double x, y, z;
            for(int i = 0; i < nAtoms; i ++){
                ifs >> id >> type >> x >> y >> z >> ilat; getline(ifs, str);
                ++ dp.cnt[ilat-1];
            }
            ofs << dp.timestep <<"\t\t\t" << dp.cnt[0] << "      " \
                << dp.cnt[1] << "      " \
                << dp.cnt[2] << "      " \
                << dp.cnt[3] << "      " \
                << dp.cnt[4] << "      " << endl;
        }
    } 
    ofs.close();
    ifs.close();
    return 0;
}

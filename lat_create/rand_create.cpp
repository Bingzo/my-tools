#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

//#define DEBUG

const double DELTA = 1E-2;

bool delta_equal( double i, double j ){
      return ( abs( i-j ) < DELTA);
}

typedef struct{
    double x;
    double y;
    double z;
}xyz;

typedef struct{
    int type;
    xyz coord;
}atom;

int check_firstneighbors( int j );

int n_atoms;
vector<atom> v_atom;

//#ifdef DEBUG
ofstream ofs;
//#endif

int main(int argc, char *argv[]){
	ifstream ifs;
	if(argc > 1){
		ifs.open(argv[1], ios_base::in);
	}
	else{
        cerr << "Please specify a input file." << endl;
        return -1;
	}
	if(!ifs){
		cerr << "File can't be opened." << endl;
		return -1;
	}

    string str;

    for(int i = 0; i < 6; i ++){
        getline(ifs, str);
    }

    int n[2];
    ifs >> n[0] >> n[1];
    getline(ifs, str);

    n_atoms = n[0] + n[1];
    getline(ifs, str);
    for(int i = 0; i < n_atoms; i ++){
        double x, y, z;
        ifs >> x >> y >> z;
        atom atm;
        atm.coord.x = x;
        atm.coord.y = y;
        atm.coord.z = z;
        v_atom.push_back(atm);
        getline(ifs, str);
    }

    ifs.close();

    int natoms_type[3] = { round( n_atoms * 0.5 ), round( n_atoms * 0 ), 
        round( n_atoms * 0.5 ) };
    int error = natoms_type[0] + natoms_type[1] + natoms_type[2] - n_atoms;
    natoms_type[0] -= error;
    //cout << natoms_type[0] << ' ' << natoms_type[1] << ' ' << natoms_type[2] << endl;

    int check_result = 0;
    int loop = 0;
    srand (time(NULL));

//#ifdef DEBUG
    ofs.open( "out.txt", ios_base::trunc );
//#endif
    while( 0 == check_result ){
        int success;
        int flag;
        vector<int> atom_index;

        if( loop % 1000 == 0)
            cout << "iteration: " << loop << endl;
        ++ loop;
        atom_index.push_back( rand() % n_atoms );
        v_atom[atom_index[0]].type = 0;
        for( int i = 0; i < natoms_type[0] - 1; i ++ ){
            success = 0;
            while( !success){
                int n = rand() % n_atoms;
                flag = 1;
                for( int j = 0; j < i; j ++ ){
                    if( n == atom_index[j] ){
                        flag = 0;
                        break;
                    }
                }
                if( 1 == flag ){
                    atom_index.push_back( n );
                    v_atom[n].type = 0;
                    success = 1;
                }
            }
        }
        for( int i = 0; i < natoms_type[1]; i ++ ){
            success = 0;
            while( !success){
                int n = rand() % n_atoms;
                flag = 1;
                for( int j = 0; j < natoms_type[0] + i; j ++ ){
                    if( n == atom_index[j] ){
                        flag = 0;
                        break;
                    }
                }
                if( 1 == flag ){
                    atom_index.push_back( n );
                    v_atom[n].type = 1;
                    success = 1;
                }
            }
        }
        for( int i = 0; i < n_atoms; i ++ ){
            flag = 1;
            for( int j = 0; j < natoms_type[0] + natoms_type[1]; j ++ ){
                if( i == atom_index[j] ){
                    flag = 0;
                    break; 
                }
            }
            if( 1 == flag ){
                atom_index.push_back( i );
                v_atom[i].type = 2;
            }
        }

#ifdef DEBUG
        ofs << "type" << endl;
        for(int i = 0; i < n_atoms; i ++){
            ofs << i << ' ' << v_atom[i].type << endl;
        }
#endif

        for(int i = 0; i < n_atoms; i ++){
            check_result = check_firstneighbors( i );
            //cout << check_result << endl;
            if(check_result == 0){
                if(loop % 1000 == 0)
                    cout << "stoped at Step " << i << endl;
                break;
            }
        }
    }
//#ifdef DEBUG
    for(int i = 0; i < n_atoms; i ++){
        ofs << i << ' ' << v_atom[i].type << endl;
    }
    ofs.close();
//#endif
}

int check_firstneighbors( int j ){
    vector<double> v_r;
    for(int i = 0; i < n_atoms; i ++){
        if( i != j ){
            xyz c0 = v_atom[j].coord;
            xyz ci = v_atom[i].coord;
            double r = sqrt( pow( c0.x-ci.x, 2 ) + pow( c0.y - ci.y, 2 ) + 
                pow( c0.z - ci.z, 2 ) );
            v_r.push_back( r );
        }
    }
    sort( v_r.begin(), v_r.end() );
    //unique( v_r.begin(), v_r.end(), delta_equal );
    for(int i = 0; i < n_atoms; i ++){
        if( i != j ){
            xyz c0 = v_atom[j].coord;
            xyz ci = v_atom[i].coord;
            double r = sqrt( pow( c0.x-ci.x, 2 ) + pow( c0.y - ci.y, 2 ) + 
                pow( c0.z - ci.z, 2 ) );
            if( r < *( v_r.begin() ) + 2*DELTA ){
#ifdef DEBUG
                ofs << j << "'s firs_nei: " << i << ' ' << v_atom[j].type << ' ' << v_atom[i].type << endl;
#endif
                if( v_atom[j].type == v_atom[i].type ){
                    return 0;
                }
            }
        }
    }
    return 1;
}

#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

const double DELTA = 1E-2;

bool delta_equal( double i, double j ){
      return ( abs( i-j ) < DELTA);
}

typedef struct{
    double x;
    double y;
    double z;
}xyz;

enum atom_t { uncertain = 0, Al, Si, O, certain };
class atom{
public:
    atom(){
        type = uncertain;
        not_type = uncertain;
        nei_considered = 0;
    }
    friend ostream &operator <<(ostream &os, atom a){
        os << a.idx << ' ' << a.type << ' ' << a.not_type << ' ' 
            << a.nei_considered << ' ' << a.coord.x << ' ' 
            << a.coord.y << ' ' << a.coord.z;
        return os;
    }
public:
    int idx;
    atom_t type;
    atom_t not_type;
    int nei_considered;
    xyz coord;
};

int get_firstneighbors(int j, int set);

int n_atoms;
vector<atom> v_atom;

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
        atm.idx = i;
        atm.coord.x = x;
        atm.coord.y = y;
        atm.coord.z = z;
        v_atom.push_back(atm);
        getline(ifs, str);
    }

    ifs.close();

    v_atom[0].type = Al;
    v_atom[0].not_type = certain;

    get_firstneighbors( 0 , 0);
    v_atom[0].nei_considered = 0;
    int idx = get_firstneighbors( 0 , 1 );
    get_firstneighbors( idx, 0 );

    ofstream ofs( "out.txt", ios_base::trunc );
    for(int i = 0; i < v_atom.size(); i ++){
        ofs << v_atom[i] << endl;
    }
    ofs.close();

	return 0;
}

int get_firstneighbors(int j, int set){
    if( v_atom[j].nei_considered == 0 && v_atom[j].not_type == certain ){
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
                    cout << j << "'s firs_nei: " << v_atom[i].idx << endl;
                    if( set == 1){
                        v_atom[i].type = Si;
                        v_atom[i].not_type = certain;
                        return v_atom[i].idx;
                    }
                    if( v_atom[i].not_type == uncertain ){
                        v_atom[i].not_type = v_atom[j].type;
                    }
                    else if( v_atom[i].not_type != certain && v_atom[i].not_type != v_atom[j].type ){
                        atom_t at;
                        if( Al == v_atom[i].not_type && Si == v_atom[j].not_type){
                            at = O;
                        }
                        if( Al == v_atom[i].not_type && O == v_atom[j].not_type){
                            at = Si;
                        }
                        if( Si == v_atom[i].not_type && O == v_atom[j].not_type){
                            at = Al;
                        }
                        v_atom[i].type = at;
                        v_atom[i].not_type = certain;
                        cout << "idx: " << i << endl;
                        get_firstneighbors( v_atom[i].idx, 0 );
                    }
                }
            }
        }
        v_atom[j].nei_considered = 1;
    }
}

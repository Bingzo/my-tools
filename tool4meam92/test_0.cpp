#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>
#include "meam92params.h"
using namespace std;

#pragma warning(disable:4996)

const char *LIB_MEAM = "library.meam";
const char *PARAM_MEAM = "paraMEAM92.txt";
const int SPACE = 32;
const int SHARP = 35;

int main(){
	int type_num;
	string *type_arr;

	cout << "How many types are in your system? ";
	cin >> type_num;
	//type_num = 2;
	cout << "The " << type_num << " types are(Au Cu...): ";
	type_arr = new string [type_num];
	for(int i = 0; i < type_num; ++ i){
		cin >> type_arr[i];
	}
	//type_arr[0] = "Cu";
	//type_arr[1] = "Au";

	ifstream ifs(LIB_MEAM, ios_base::in);

	if(ifs.fail()){
		cerr << "There is no libaray file exited in current directory." << endl;
		return -1;
	}

	int iret_peek;
	string str;
	double d_dis;
	MEAM92Params *params = new MEAM92Params(type_num);

	params->nelt = type_num;
	for(int i = 0; i < type_num; i++){
		params->eltype[i] = type_arr[i];
	}
	
	int ind = 0;
	while(!ifs.eof()){
		iret_peek = ifs.peek();
		if(SPACE == iret_peek || SHARP == iret_peek){
			getline(ifs, str);
		}
		else {
			ifs >> str;
			if(0 == str.compare(1, str.size() - 2, type_arr[ind])){
				ifs >> str;	
				char buffer[8]; 

				str.copy(buffer, str.size() - 2, 1); 
				buffer[str.size() - 2] = '\0';
				params->latType[ind][ind] = buffer;
				ifs >> d_dis;
				params->Z[ind] = (int)d_dis;
				ifs >> params->ielt[ind];
				getline(ifs, str);
				ifs >> params->alpha[ind][ind] >> params->beta[0][ind] >> params->beta[1][ind] \
					>> params->beta[2][ind] >> params->beta[3][ind] >> params->re[ind][ind] \
					>> params->Ec[ind][ind] >> params->A[ind];
				getline(ifs, str);
				ifs >> params->t[0][ind] >> params->t[1][ind] >> params->t[2][ind] \
					>> params->t[3][ind] >> params->rozero[ind] >> params->gscheme[ind];
				getline(ifs, str);
				switch(params->gscheme[ind]){
					case 0: 
					case 1:
						++ params->gscheme[ind];
						break;
					case 2:
						params->gscheme[ind] = 0;
						cerr << "ibar equals 2 int the library" << endl;
						break;
					default:
						break;
				}
				if(0 == params->latType[ind][ind].compare(0, sizeof("fcc") - 1, "fcc")){
					params->re[ind][ind] /= sqrt(2.0);
				}
				else if(0 == params->latType[ind][ind].compare(0, sizeof("bcc") - 1, "bcc")){
					params->re[ind][ind] *= sqrt(3.0)/2.0;
				}
				else if(0 == params->latType[ind][ind].compare(0, sizeof("dia") - 1, "dia")){
					params->re[ind][ind] *= sqrt(3.0)/4.0;
				}
				params->MEAM92ParamsRearrange();

				if(type_num == ++ ind){
					break;
				}
				ifs.seekg(0);
			}
		}
	}
	ifs.close();

	ofstream ofs(PARAM_MEAM, ios_base::trunc);
	ofs << fixed << setprecision(8);
	ofs << "# Comment line      # All the lines with a leading \"#\" are regarded as comment line." << endl;
	ofs << "numOfTypes" << "\t" << params->nelt << endl;
	ofs << endl;

	for(int i = 0; i < params->nelt; i++){
		ofs << "BEGIN ELEMENT" << endl;
		ofs << "# basic parameters (MUST!)" << endl;
		ofs << "elt" << "\t" << params->eltype[i] << "\t" << "# element symbol" << endl;
		ofs << "ielt" << "\t" << params->ielt[i] << "\t" << "# atomic number of element" << endl;
		ofs << "lat" << "\t" << params->latType[i][i] << "\t" << "# reference structure; Options: fcc,bcc,dia,hcp,dim" << endl;
		ofs << "Z" << "\t" << params->Z[i] << "\t" << "# number of first-nearest neighbors in the reference structure" << endl;
		ofs << "E0" << "\t" << params->Ec[i][i] << "\t" << "# sublimation energy; esub in LAMMPS" << endl;
		ofs << "R0" << "\t" << params->re[i][i] << "\t" << "# nearest neighbor distance; similar with alat in LAMMPS" << endl;
		ofs << "a" << "\t" << params->alpha[i][i] << "\t" << "# alpha; alat in LAMMPS" << endl;
		ofs << "A" << "\t" << params->A[i] << "\t" << "# A; asub in LAMMPS" << endl;
		ofs << "b0" << "\t" << params->beta[0][i] << "\t" << "# beta0" << endl;
		ofs << "b1" << "\t" << params->beta[1][i] << "\t" << "# beta1" << endl;
		ofs << "b2" << "\t" << params->beta[2][i] << "\t" << "# beta2" << endl;
		ofs << "b3" << "\t" << params->beta[3][i] << "\t" << "# beta3" << endl;
		ofs << "t0" << "\t" << params->t[0][i] << "\t" << "# t0" << endl;
		ofs << "t1" << "\t" << params->t[1][i] << "\t" << "# t1" << endl;
		ofs << "t2" << "\t" << params->t[2][i] << "\t" << "# t2" << endl;
		ofs << "t3" << "\t" << params->t[3][i] << "\t" << "# t3" << endl;
		ofs << endl;
		ofs << "# auxiliary parameters (If supplied, default values will be rewritten.)" << endl;
		ofs << "rho0" << "\t" << params->rozero[i] << "\t" << "# density scaling parameter; rozero in LAMMPS; default = 1.0" << endl;
		ofs << "gscheme" << "\t" << params->gscheme[i] << "\t" << "# density scaling parameter; rozero in LAMMPS; default = 1.0" << endl;
		ofs << "\t\t\t" << "# 1: G = 2/(1+exp(-Gamma)) (default)" << endl;
		ofs << "\t\t\t" << "# 2: G = sqrt(1+Gamma)" << endl;
		ofs << "\t\t\t" << "# 3: G = exp(Gamma/2)" << endl;
		ofs << "END ELEMENT" << endl;
		ofs << endl;
 	}

	int dis_mark = 0;
	ofs << "BEGIN PAIR" << endl;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "nn2(" << i+1 << "," << j+1 << ")" << "\t" << params->nn2[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# switch for whether to use second-nearest neighbor" << endl;
				ofs << "\t\t\t" << "# 0: don't use 2nd-nearest neighbor (default for all)" << endl;
				ofs << "\t\t\t" << "# 1: use 2nd-nearest neighbor";
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "zbl(" << i+1 << "," << j+1 << ")" << "\t" << params->zbl[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# switch for whether using ZBL potential for small atom separations." << endl;
				ofs << "\t\t\t" << "# 0: don't use ZBL potential." << endl;
				ofs << "\t\t\t" << "# 1: use ZBL potential (default for all)";
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "lattice(" << i+1 << "," << j+1 << ")" << "\t" << params->latType[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# lattice structure of I-J reference structure" << endl;
				ofs << "\t\t\t" << "# options: dia,fcc,bcc,dim,b1,hcp,c11,l12,b2 (refer to LAMMPS)";
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "Ec(" << i+1 << "," << j+1 << ")" << "\t" << params->re[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# cohesive energy of reference structure for I-J mixture";
				dis_mark = 1;
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "a(" << i+1 << "," << j+1 << ")" << "\t" << params->alpha[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# alpha parameter for pair potential between I and J";
				dis_mark = 1;
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			ofs << "re(" << i+1 << "," << j+1 << ")" << "\t" << params->re[i][j];
			if(0 == dis_mark){
				ofs << "\t" << "# equilibrium distance between I and J in reference structure";
				dis_mark = 1;
			}
			ofs << endl;
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			for(int k = 0; k < params->nelt; ++ k){
				ofs << "Cmax(" << i+1 << "," << j+1 << "," << k+1 <<")" << "\t" << params->Cmax[i][j][k];
				if(0 == dis_mark){
					ofs << "\t" << "# Cmax when I-J pair is screened by K (I<=J); default = 2.8";
					dis_mark = 1;
				}
				ofs << endl;
			}
		}
	}
	dis_mark = 0;
	for(int i = 0; i < params->nelt - 1; ++ i){
		for(int j = i + 1; j < params->nelt; ++ j){
			for(int k = 0; k < params->nelt; ++ k){
				ofs << "Cmin(" << i+1 << "," << j+1 << "," << k+1 <<")" << "\t" << params->Cmin[i][j][k];
				if(0 == dis_mark){
					ofs << "\t" << "# Cmin when I-J pair is screened by K (I<=J); default = 2.0";
					dis_mark = 1;
				}
				ofs << endl;
			}
		}
	}
	ofs << "END PAIR" << endl;
	ofs << endl;

	ofs << "BEGIN GLOBAL" << endl;
	ofs << "# global parameters" << endl;
	ofs << "rc" << "\t" << params->rc << "\t"<< "# cutoff radius for cufoff function (default 4.0)" << endl;
	ofs << "delr" << "\t" << params->delr<< "\t" << "# length of smoothing distance for cutoff function (default 0.1)" << endl;
	ofs << "augt1" << "\t" << params->augt1 << "\t"<< "# switch for whether to augment t1 parameter" << endl;
	ofs << "\t\t\t" << "# 0: don't augment t1" << endl;
	ofs << "\t\t\t" << "# 1: augment t1 by 3/5*t3 (default)" << endl;
	ofs << "END GLOBAL" << endl;

	ofs.close();

	delete params;
	delete []type_arr;
	
	//system("pause");
	return 0;
}


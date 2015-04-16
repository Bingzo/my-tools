/**
@file	meam92params.h
header file for MEAM92
@brief last modified date: 2012.11.19 
*/

#include "meam92params.h"
/*!
@brief constructor, allocate memory
*/
MEAM92Params::MEAM92Params(){
	_init(1);
}
MEAM92Params::MEAM92Params(int n){
	_init(n);
}
/*!
@brief destructor, release allocated memory
*/

MEAM92Params::~MEAM92Params(){
	_del();
}

/*!
@brief resize allocated memory
@note should be called after nelt(the total number of elements) has been read from the input file.
*/
void MEAM92Params::MEAM92ParamsRealloc(int n){
	_del();
	_init(n);
}

/*!
@brief rearrange the characters of the struct to make matrix symmetrical
@note should be called after all the parameters has been read from the input file.
*/
void MEAM92Params::MEAM92ParamsRearrange(){
	for(int i = 0; i < nelt; i++){
		t[1][i] += augt1 * 3.0 / 5 * t[3][i];
	}

	for(int i = 0; i < nelt - 1; i++){
		for(int j = i + 1; j < nelt; j++){
			if(equal_double(re[i][j], -1.0)){
				re[i][j] = (re[i][i] + re[j][j]) / 2;
			}
			if(equal_double(alpha[i][j], -1.0)){
				alpha[i][j] = (alpha[i][i] + alpha[j][j]) / 2;
			}
			if(equal_double(Ec[i][j], -1.0)){
				Ec[i][j] = (Ec[i][i] + Ec[j][j]) / 2;
			}
		}
	}
	for(int i = 1; i < nelt; i++){
		for(int j = 0; j < i; j++){
			re[i][j] = re[j][i];
			alpha[i][j] = alpha[j][i];
			Ec[i][j] = Ec[j][i];
		}
	}

	for(int i = 0; i < nelt - 1; i++){
		for(int j = i + 1; j < nelt; j++){
			if(latType[i][i] == latType[j][j]){
				latType[i][j] = latType[i][i];
			}
		}
	}
	for(int i = 1; i < nelt; i++){
		for(int j = 0; j < i; j++){
			latType[i][j] = latType[j][i];
		}
	}

	for(int i = 1; i < nelt; i++){
		for(int j = 0; j < i; j++){
			for(int k = 0; k < nelt; k++){
				Cmax[i][j][k] = Cmax[j][i][k];
				Cmin[i][j][k] = Cmin[j][i][k];
			}
		}
	}
	//rho0 is left behind.
}

void MEAM92Params::_init(int n){
	assert(n > 0);
	nelt = n;
	eltype = new std::string [nelt];
	for(int i = 0; i < nelt; i++){
		eltype[i] = "NULL";
	}
	ielt = new AtomicNumber[nelt];
	for(int i = 0; i < nelt; i++){
		ielt[i] = -1;
	}
	latType = new std::string *[nelt];
	for(int i = 0; i< nelt; i++){
		latType[i] = new std::string[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			latType[i][j] = "NULL";
		}	
	}
	Z = new int[nelt];
	for(int i = 0; i < nelt; i++){
		Z[i] = -1;
	}
	Ec = new double *[nelt];
	for(int i = 0; i < nelt; i++){
		Ec[i] = new double[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			Ec[i][j] = -1.0;
		}
	}
	re = new double *[nelt];
	for(int i = 0; i < nelt; i++){
		re[i] = new double[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			re[i][j] = -1.0;
		}		
	}
	alpha = new double *[nelt];
	for (int i = 0; i < nelt; i++){
		alpha[i] = new double[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			alpha[i][j] = -1.0;
		}		
	}
	A = new double[nelt];
	for(int i = 0; i < nelt; i++){
		A[i] = -1.0;
	}
	for(int i = 0; i < 4; i++){
		beta[i] = new double[nelt];
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < nelt; j++){
			beta[i][j] = -1.0;
		}
	}
	for(int i = 0; i< 4; i++){
		t[i] = new double[nelt];
	}
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < nelt; j++){
			t[i][j] = -1.0;
		}
	}

	rozero = new double[nelt];
	for (int i = 0; i < nelt; i++){
		rozero[i] = 1.0;
	}

	gscheme = new int[nelt];
	for(int i = 0; i < nelt; i++){
		gscheme[i] = 1;
	}

	nn2 = new int *[nelt];
	for(int i = 0; i < nelt; i++){
		nn2[i] = new int[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			nn2[i][j] = 1;
		}
	}
	zbl = new int *[nelt];
	for(int i = 0; i < nelt; i++){
		zbl[i] = new int[nelt];
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			zbl[i][j] = 1;
		}
	}
	Cmax = new double **[nelt];
	for(int i = 0; i < nelt; i++){
		Cmax[i] = new double *[nelt];
		for(int j = 0; j < nelt; j++){
			Cmax[i][j] = new double[nelt];
		}
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			for(int k = 0; k < nelt; k++){
				Cmax[i][j][k] = 2.8;
			}
		}
	}
	Cmin = new double **[nelt];
	for(int i = 0; i < nelt; i++){
		Cmin[i] = new double *[nelt];
		for(int j = 0; j < nelt; j++){
			Cmin[i][j] = new double[nelt];
		}
	}
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			for(int k = 0; k < nelt; k++){
				Cmin[i][j][k] = 2.0;
			}
		}
	}

	rc = 4.0;
	delr = 0.1;
	augt1 = 1;
}

void MEAM92Params::_del(){
	delete []eltype;
	delete []ielt;	
	for(int i = 0; i< nelt; i++){
		delete []latType[i];
	}
	delete []latType;
	delete []Z;
	for(int i = 0; i < nelt; i++){
		delete []Ec[i];
	}
	delete []Ec;
	for(int i = 0; i < nelt; i++){
		delete []re[i];
	}
	delete []re;	
	for (int i = 0; i < nelt; i++){
		delete []alpha[i];
	}
	delete []alpha;
	delete []A;
	for(int i = 0; i < 4; i++){
		delete []beta[i];
	}
	for(int i = 0; i< 4; i++){
		delete []t[i];
	}

	delete []rozero;
	delete []gscheme;

	for(int i = 0; i < nelt; i++){
		delete []nn2[i];
	}
	delete []nn2;	
	for(int i = 0; i < nelt; i++){
		delete []zbl[i];
	}
	delete []zbl;
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			delete []Cmax[i][j];
		}
	}
	for(int i = 0; i < nelt; i++){
		delete []Cmax[i];
	}
	delete []Cmax;
	for(int i = 0; i < nelt; i++){
		for(int j = 0; j < nelt; j++){
			delete []Cmin[i][j];
		}
	}
	for(int i = 0; i < nelt; i++){
		delete []Cmin[i];
	}
	delete []Cmin;
}

int MEAM92Params::equal_double(double ed0, double ed1){
	double diff;
	diff = ed0 - ed1;
	diff = (diff > 0)? diff : (-diff);
	return diff < (1e-6);
}
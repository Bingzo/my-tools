/**
 @file	meam92params.h
 header file for MEAM92
 @brief last modified date: 2012.11.19 
*/

#ifndef _MEAM92PARAMS_H__
#define _MEAM92PARAMS_H__

#include <string>
#include "global.h"
#include <assert.h>
typedef struct MEAM92Params{
public:
	//Constructor
	MEAM92Params();
	MEAM92Params(int n);
	//Destructor
	virtual ~MEAM92Params();

	/// Resize the allocated memory according to the number of atoms in the input file.
	void MEAM92ParamsRealloc(int n);
	/// Rearrange the characters of the struct to make matrix symmetrical.
	void MEAM92ParamsRearrange();

	int nelt;                    ///the total number of element types (default 1;maximum: 5)
	std::string *eltype;         ///the element type, such as "Au"
	AtomicNumber *ielt;          ///the No. of the elements
	std::string **latType;       ///the lattice Type, such as "fcc"
	//????????????Does this 
	//typedef enum{CP, FCC, BCC}LatticType;
	//vector<LatticeType> latType;
	//????????????look better?
	int  *Z;                    ///the number of the first-nearest atoms
	double **Ec;                ///E0,sublimation energy
	double **re;                ///R0, nearest neighbor distance
	double **alpha;             ///a;alpha
	double *A;                  ///A;parameter
	double *beta[4];            ///beta0~3
	double *t[4];               ///t0~3

	double *rozero;             ///rho0
	int *gscheme;               ///options for Gamma function(default 1)

	int **nn2;                  ///switch for whether consider the effect of second-nearest atoms(default 0)
	int **zbl;                  ///switch for whether use the ZBL function(default 1)
	double ***Cmax;             ///default 2.8
	double ***Cmin;             ///default 2.0

	double rc;                  ///cutoff radius for cutoff function(default 4.0)
	double delr;                ///length of smoothing distance for cutoff function(default 0.1)
	int augt1;                  ///switch for whether to augment t1 parameter(default 1)
private:
	void _init(int n);
	void _del();
	int equal_double(double ed0, double ed1);
}MEAM92Params;

#endif
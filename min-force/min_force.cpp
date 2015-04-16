/**
 * @file min_force.cpp
 * @Synopsis  calculate the minimum point of conversitive force of 
 *              DPD and FENE
 * @author Bing Zhou   zhoubinghust@126.com
 * @version 1.0.0
 * @date 2014-08-11
 */
#include <iostream>
#include <cmath>
#include <limits>
using namespace std;

const double dpd_a = 18.75;
const double dpd_rc = 1.0;
const double fene_H = 10.0;
const double fene_Q0 = 1.0;

int main(){
    const int N = 100;
    double r[N];
    double F[N];
    for(int i = 0; i < N; i ++){
        r[i] = i / (N*dpd_rc);
        F[i] = dpd_a * (1.0 - r[i]/dpd_rc) + fene_H * r[i] / (1 - pow(r[i]/fene_Q0, 2.0));
    }
    double min_r;
    double min_f = numeric_limits<double>::max();
    for(int i = 0; i < N; i ++){
        if(min_f > F[i]){
            min_f = F[i];
            min_r = r[i];
        }
    }
    cout << "minimum point is: ( " << min_r << ", " << min_f << " )." << endl;
    
    double k = fene_H * dpd_rc / dpd_a;
    min_r = sqrt(1 - 0.5 * (sqrt(k) * (sqrt(8+k) - sqrt(k))));
    min_r *= fene_Q0;
    cout << "minimum r is: " << min_r << endl;

    const int kN = 100;
    double ki[kN];
    double yi[kN];
    for(int i = 0; i < kN; i ++){
        ki[i] = i / (kN*1.0) + 1e-4; 
        yi[i] = 0.25 / sqrt(1 - 0.5 * (sqrt(ki[i]) * (sqrt(8+ki[i]) - sqrt(ki[i])))) 
            * (1 - (4 + ki[i]) / sqrt(ki[i]*ki[i] + 8*ki[i]));
        //cout << yi[i] << ' ';
    }
    //cout << endl;
    double max_ki = -1.0;
    double max_yi = -numeric_limits<double>::max();
    for(int i = 0; i < kN; i ++){
        if(max_yi < yi[i]){
            max_yi = yi[i];
            max_ki = ki[i];
        }
    }

    cout << "best k is: " << max_ki << endl;
    //cout << "yi: " << max_yi << endl;

    return 0;
}

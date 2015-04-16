#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <complex>
using namespace std;

int main(int argc, char *argv[]){
    ifstream ifs;
    ofstream ofs;

    if(argc == 1){
            ifs.open("pure_phase.txt", ios_base::in);
    }
    else{
            ifs.open(argv[1], ios_base::in);
    }

	ofs.open("out.txt", ios_base::trunc);

	ofs << "x" << "\t" << "I" << "\t" << "R" << "\t" << "a" << "\t" << "n" << "\t" << "L" << "\t" << "aE" << endl;
	while(!ifs.eof()){
		double data[8];
		double out[2];
		string str;

		for(int i = 0; i < 8; i++){
			ifs >> data[i];
		}
		getline(ifs, str);
		out[0] = 1240.0 / data[0];
		double sum[2] = {0.0, 0.0};
		for(int i = 1; i < 4; i ++){
			sum[0] += data[i];
		}
		sum[0] /= 3.0;
		for(int i = 5; i < 8; i ++){
			sum[1] += data[i];
		}
		sum[1] /= 3.0;

		double w = 1.52 * pow(10.0, 15) * data[0];
		double temp = sqrt(pow(sum[1], 2.0) + pow(sum[0], 2.0));
		temp -= sum[1];
		double I = 2*w*sqrt(temp/2);
		out[1] = I;
		
		complex<double> c(sum[1], sum[0]);
		double R = norm(sqrt(c) - 1.0) / norm(sqrt(c + 1.0));
		double a = sqrt(2.0) * w * sqrt(sqrt(pow(sum[1], 2.0) + pow(sum[0], 2.0)) - sum[1]);
		double n = sqrt(sqrt(pow(sum[1], 2.0) + pow(sum[0], 2.0)) + sum[1]) / sqrt(2);
		double L = sum[0] / (pow(sum[1], 2.0) + pow(sum[0], 2.0));
		double aE = a * data[0];

		ofs << out[0] << "\t" << out[1] << "\t" << R << "\t" << a << "\t" << n << "\t" << L << "\t" << aE << endl;
	}

	ifs.close();
	ofs.close();

	return 0;
}
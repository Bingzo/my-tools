#include<fstream>
#include<string>
#include<iostream>
using namespace std;

int main(int argc, char *argv[]){
	ifstream ifs(argv[1]);
//	ifstream ifs("out.asub");
	string str;
	string str_dis;
	double xi[6];
	double pp[3];
	while(!ifs.eof()){
		getline(ifs, str);
		if(0 == str.compare("xi =")){
			getline(ifs, str_dis);
			for(int i = 0; i < 6; i++){
				ifs >> xi[i];
			}
			for(int i = 0; i < 4; i++){
				getline(ifs, str_dis);
			}
			for(int i = 0; i < 3; i ++){
				ifs >> str_dis >> pp[i];
			}
			for(int i = 0; i < 6; i++){
				cout << xi[i] << ' ';
			}
			for(int i = 0; i < 3; i++){
				cout << pp[i] << ' ';
			}
			cout << endl;
		}
	}
	system("pause");
}

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Coord{
	int atomType;
	double x;
	double y;
	double z;
};

struct lessFunc{
	bool operator() (Coord &c0, Coord &c1){
		return c0.atomType < c1.atomType;
	}
}lessfunc;

int main(int argc, char *argv[]){
	ifstream fMincryst;
	if(argc < 2){
		cerr << "Input file not found." << endl;
		return -1;
	}
	else{
		fMincryst.open(argv[1], ios_base::in);
		if(fMincryst.bad()){
			cerr << "Input file not found." << endl;
			return -1;
		}
	}
	//fMincryst.open("in.CoSb", ios_base::in);

	ofstream fPoscar;
	string inStr;
	size_t found;
	string eName;
	int nEle;
	while(!fMincryst.eof()){
		getline(fMincryst, inStr);

		found = inStr.find("W W W - X R A Y P O L");
		if(found != string::npos){
			int i = 3;
			while(i --){
				getline(fMincryst, inStr);
			}
			int iNum;
			fMincryst >> iNum; getline(fMincryst, inStr);
			getline(fMincryst, inStr);
			getline(fMincryst,inStr);
			eName = inStr;
			char fName[64];
			sprintf(fName, "POSCAR_%s", inStr.c_str());
			fPoscar.open(fName, ios_base::trunc);
			if(fPoscar.bad()){
				cerr << "File not in the correct format" << endl;
				return -1;
			}
			fPoscar << inStr << endl;
			fPoscar << iNum << endl;
		}

		found = inStr.find("Lattice parameters");
		if(found != string::npos){
			char ch[2];
			double value;
			int i = 3;
			getline(fMincryst, inStr);
			while(i --){
				fMincryst >> ch[0] >> ch[1] >> value; getline(fMincryst, inStr);
				switch (ch[0])
				{
				case 'a':
					fPoscar << value << ' ' << 0.0  << ' ' << 0.0 << endl;
					break;
				case 'b':
					fPoscar << 0.0 << ' ' << value  << ' ' << 0.0 << endl;
					break;
				case 'c':
					fPoscar << 0.0  << ' ' << 0.0 << ' ' << value << endl;
					break;
				default:
					break;
				}
			}
			vector<string> vName;
			string sTmp;
			int pos;
			pos = eName.find_first_of('(');
			if(pos != string::npos){
				eName = eName.substr(0, pos);
			}
			sTmp += eName[0];
			for(int i = 1; i < eName.length(); i ++){
				if(eName[i] > 'a' && eName[i] < 'z'){
					sTmp += eName[i];
				}
				if(eName[i] > 'A' && eName[i] < 'Z'){
					vName.push_back(sTmp);
					sTmp.clear();
					sTmp += eName[i];
				}
			}
			vName.push_back(sTmp);
			nEle = vName.size();
			vector<string>::iterator it;
			for(it = vName.begin(); it != vName.end(); it ++){
				fPoscar << *it << ' ';
			}
			fPoscar << endl;
		}

		found = inStr.find("Co-ordinates for all atomic positions :");
		if(found != string::npos){
			vector<Coord> vCoord;
			int i = 3;
			while(i --){
				getline(fMincryst, inStr);
			}
			char c;
			fMincryst.get(c);
			while(' ' == c){
				int n;
				Coord coord;
				fMincryst >> n >> coord.atomType >> coord.x >> coord.y >> coord.z; getline(fMincryst, inStr);
				vCoord.push_back(coord);
				fMincryst.get(c);
			}
			sort(vCoord.begin(), vCoord.end(), lessfunc);
			vector<Coord>::iterator it;
			vector<int> vTmp;
			for(it = vCoord.begin(); it != vCoord.end(); it ++){
				vTmp.push_back((*it).atomType);
			}
			for(int i = 0; i < nEle; i++){
				fPoscar << count(vTmp.begin(), vTmp.end(), i+1) << ' ';			
			}
			fPoscar << endl;
			fPoscar << "Direct" << endl;
			for(it = vCoord.begin(); it != vCoord.end(); it ++){
				fPoscar << (*it).x << ' '<< (*it).y << ' '<< (*it).z << endl;
			}
			break;
		}
	}

	fMincryst.close();
	fPoscar.close();

	system("pause");
	return 0;
}
#include <iostream>
#include <vector>
#define cimg_use_png 1
#include "CImg.h"
#include <random>
#include <chrono>
#include <algorithm>
#include <filesystem>

using namespace cimg_library;
using namespace std;

vector<vector<int>> tests = vector<vector<int>>(7);
vector<string> folders = {"anger","contempt","disgust","fear","happy","sadness","surprise"};
string basePath = "/Users/panflete/Documents/UTEC/Ciclo 6/EDA/kdtree_recognition/CK+48/";

vector<double> vectorize(CImg<double> &img, int m) {
	CImg<double> haar = img.haar(false, m);
	CImg<double> crop = haar.crop(0, 0, 32, 32);
	
	vector<double> result;

	cimg_forXY(crop, x, y) { 
		result.push_back((crop(x, y, 0) + crop(x, y, 1) + crop(x, y, 2)) / 3);
	}

	return result;
}

vector<pair<string,vector<double>>> get_vectors() {

	vector<pair<string,vector<double>>> points;
	
	vector<int> numFiles = {135,54,177,75,207,84,249};

	unsigned seed = chrono::system_clock::now().time_since_epoch().count();

	vector<int> random_files;
	
	int cur = 0;

	for(int i = 0; i < folders.size(); i++) {

		cout<<"Vectorizing: "<<folders[i]<<"\n";

		//Generamos todos los indices de las imagenes
		for(int j = 0; j < numFiles[i]; j++) {
			random_files.push_back(j);
		}

		shuffle(random_files.begin(), random_files.end(), default_random_engine(seed));

		//Guardamos los indices de las imagenes que usamos para los tests
		for(int j = numFiles[i]*0.7; j < numFiles[i]; j++) {
			tests[i].push_back(random_files[j]);
		}

		//Cortamos las imagenes de test
		random_files.resize((int)(numFiles[i]*0.7));

		sort(random_files.begin(), random_files.end());

		for(const auto & entry : filesystem::directory_iterator(basePath+folders[i])) {
			
			//Si es una imagen para tests no la agregamos
			if(find(random_files.begin(),random_files.end(),cur) == random_files.end()) {
				cur++;
				continue;
			}

			string entry_path = basePath+folders[i]+"/"+entry.path().filename().string();
			CImg<double> A(entry_path.c_str());
			A.resize(48,48);
			vector<double> vA = vectorize(A,3);

			points.push_back({folders[i],vA});

			cur++;
		
		}

		random_files.clear();
		cur = 0;

	}

	return points;
	
}

int main() {

	vector<pair<string,vector<double>>> points = get_vectors();

	cout<<points[0].second.size()<<endl;

	return 0;
}
#include <iostream>
#include <vector>
#define cimg_use_png
#include "CImg.h"
#include <random>
#include <time.h>

using namespace cimg_library;
using namespace std;
using std::filesystem::directory_iterator;

vector<vector<int>> tests;
vector<string> folders = {"anger","contempt","disgust","fear","happy","sadness","surprise"};
string basePath = "D:/Documents/UTEC/Ciclo 6/EDA/kdtree_recognition/CK+48/";

vector<double> vectorize(CImg<double> &img, int m) {
	CImg<double> haar = img.haar(false, m);
	CImg<double> crop = haar.crop(0, 0, 27, 27);
	
	vector<double> result;

	cimg_forXY(crop, x, y) { 
		result.push_back((crop(x, y, 0) + crop(x, y, 1) + crop(x, y, 2)) / 3);
	}

	return result;
}

vector<vector<pair<string,double>>> get_vectors() {

	vector<vector<pair<string,double>>> points;
	
	vector<int> numFiles = {135,54,177,75,207,84,249};

	srand(time(NULL));

	vector<int> random_files;
	
	int cur = 0;

	for(int i = 0; i < folders.size(); i++) {

		//Generamos todos los indices de las imagenes
		for(int j = 0; j < numFiles[i]; j++) {
			random_files.push_back(j);
		}

		random_shuffle(random_files.begin(), random_files.end());

		//Guardamos los indices de las imagenes que usamos para los tests
		for(int j = numFiles[i]*0.7; j < numFiles[i]; j++) {
			tests[i].push_back(random_files[j]);
		}

		//Cortamos las imagenes de test
		random_files.resize((int)(numFiles[i]*0.7));

		sort(random_files.begin(), random_files.end());

		for(const auto & entry : fs::directory_iterator(basePath+folders[i])) {
			
			//Si es una imagen para tests no la agregamos
			if(find(cur,random_files.begin(),random_files.end()) == random_files.end()) {
				cur++;
				continue;
			}

			string entry_path = basePath+f+"/"+entry.path().filename().string();
			CImg<double> A(entry_path.c_str());
			vector<double> vA = vectorize(A,4);

			points.push_back({f,vA});

			cur++;
		
		}

		random_files.clear();
		cur = 0;

	}

	return points;
	
}

int main() {


	auto points = get_vectors();

	return 0;
}
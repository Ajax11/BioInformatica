
#include <iostream>
#include <vector>
#include <algorithm>    // std::swap
#include <utility> //std::pair
#include <math.h>
#include <fstream>
#include <string>

#include <ctime>
#include <ratio>
#include <chrono>

/*
	Making The cluster with a Matrix of Distance
*/

#define testNumGen 7
#define resourcePath "DataSet/"
#define resultPath "Resultados/"

struct cluster{
	
	cluster(){
		for (int i = 0; i < testNumGen; ++i){
			std::string gen = "";
			gen += (int('A') + i); 
			gens.push_back( gen );
		}
		matrixOfDistance.resize(testNumGen);
		std::string file = "";
		file += resourcePath;
		file += "testDistance.txt";
		std::ifstream stream( file.c_str() );
		std::string buffer;
		getline(stream, buffer , '\n');
		int i = 0;
		while(stream){
			while(buffer.size() != 0){
				std::string::size_type sizeOfTheNumber;
				float number = std::stof (buffer,&sizeOfTheNumber);
				buffer = buffer.substr(sizeOfTheNumber);
				matrixOfDistance[i].push_back(number);
			}
			getline(stream, buffer , '\n');
			++i;
		}
		stream.close();
	}

	cluster(std::string file){
		std::ifstream stream( file.c_str() );
		std::string buffer;
		getline(stream, buffer , '\n');
		getline(stream, buffer , '\n');

		std::vector< std::vector<float> > vectorOfPointsOfTheGen;
		while(stream){
			gens.push_back(buffer.substr(0,buffer.find('\t')));
			buffer = buffer.substr(buffer.find('\t'));
			std::vector<float> pointOfTheGen;
			while(buffer.size() > 1){
				std::string::size_type sizeOfTheNumber;
				float number = std::stof (buffer,&sizeOfTheNumber);
				buffer = buffer.substr(sizeOfTheNumber);
				pointOfTheGen.push_back(number);
			}
			vectorOfPointsOfTheGen.push_back(pointOfTheGen);
			getline(stream, buffer , '\n');
		}
		stream.close();
		matrixOfDistance.resize(gens.size());
		for (int i = 0; i < gens.size(); ++i){
			matrixOfDistance[i].resize(gens.size(),0.0);
		}
		for (int i = 0; i < matrixOfDistance.size(); ++i){
			for (int j = i; j < matrixOfDistance[i].size(); ++j){
				if (i == j){
					matrixOfDistance[i][j] = 0.0;
				} else {
					float distance = 0.0;
					for (int k = 0; k < vectorOfPointsOfTheGen[i].size(); ++k){
						float tmp = vectorOfPointsOfTheGen[j][k] - vectorOfPointsOfTheGen[i][k];
						distance += (tmp * tmp);
					}
					distance = sqrt(distance);
					matrixOfDistance[i][j] = distance;
					matrixOfDistance[j][i] = distance;
				}
			}
		}
	}

	std::vector< std::string > gens;
	std::vector< std::vector<float> > matrixOfDistance;
	std::vector<std::string> genCluster;
	int numberSteps;

	void showCluster(){
		std::cout<<"The clusters are: "<<'\n';
		for (int i = 0; i < genCluster.size(); ++i){
			std::cout << "Cluster N° " << i+1 << '\n' << genCluster[i] << '\n';
		}
	}

	void saveCluster(std::string file = ""){
		if (file.size() == 0){
			file += resultPath;
			file += "result-Jerarquico-Disociativo.txt";
		}

		std::ifstream stream( file );
		std::string buffer;
		getline(stream, buffer, '\n');
		stream.close();

		std::ofstream outputFile;
		outputFile.open( file.c_str() );
		outputFile << "Method: Clustering Jerarquico Disociativo" << '\n' << buffer << '\n' << '\n';
		outputFile << "The clusters are: " << '\n' << '\n';
		for (int i = 0; i < genCluster.size(); ++i){
			outputFile << "Cluster N° " << i+1 << '\n' << genCluster[i] << '\n';
		}
		outputFile.close();
	}

	void setGens(std::string gen = "A"){
		gens.push_back(gen);
	}

	void doCluster(int flag = -1, std::string file = ""){
		numberSteps = flag;
		auto begin = std::chrono::high_resolution_clock::now();
		makeCluster();
		auto end = std::chrono::high_resolution_clock::now();
		//showCluster();
		std::ofstream outputFile;
		if (file.size() == 0){
			file += resultPath;
			file += "result-Jerarquico-Disociativo.txt";
		}

		outputFile.open(file.c_str());
		outputFile << "Time:" << '\t';
		outputFile << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns" <<'\n';
		outputFile.close();
		saveCluster(file);
	}

	void makeCluster(){
		std::vector< std::pair<int,float> > arrayDistanceGen;
		std::pair<int,float> maxPair; 
		for (int i = 0; i < matrixOfDistance.size(); ++i){
			float count = (i)?matrixOfDistance[i][0]:matrixOfDistance[i][1];
			for (int j = 0; j < matrixOfDistance[i].size(); ++j){
				if ( i == j )
					continue;
				count = (count > matrixOfDistance[i][j])?matrixOfDistance[i][j]:count;
			}
			arrayDistanceGen.push_back(std::pair<int,float>(i,count));
		}

		/*
			sort the array
		*/
		for (int i = 0; i < arrayDistanceGen.size(); ++i){
			for (int j = i+1; j < arrayDistanceGen.size(); ++j){
				if(arrayDistanceGen[i].second < arrayDistanceGen[j].second)
					std::swap(arrayDistanceGen[i],arrayDistanceGen[j]);
			}
		}

		bool flag = 1;
		for (int i = 1; (arrayDistanceGen.size() != 0) && flag; ++i){
			maxPair.first = arrayDistanceGen[0].first;
			maxPair.second = arrayDistanceGen[0].second;
			std::string clusterGen = "";
			clusterGen += '\t';
			clusterGen += gens[maxPair.first];
			clusterGen += '\n';
			arrayDistanceGen.erase(arrayDistanceGen.begin());
			std::vector<int> vectorToDeleteGens;
			for (int i = 0; i < arrayDistanceGen.size(); ++i){
				float additionFlag = arrayDistanceGen[i].second - matrixOfDistance[maxPair.first][arrayDistanceGen[i].first];
				/*
					Comparison to verify if the element is inside or outside of the selected cluster
				*/
				if (additionFlag >= 0){
					vectorToDeleteGens.push_back(i);
				}
			}
			for (int i = vectorToDeleteGens.size() - 1; (vectorToDeleteGens.size() != 0) && (i >= 0); --i){
				clusterGen += '\t';
				clusterGen += gens[arrayDistanceGen[i].first];
				clusterGen += '\n';
				arrayDistanceGen.erase(arrayDistanceGen.begin() + vectorToDeleteGens[i]);
			}
			genCluster.push_back(clusterGen);
			flag = (numberSteps == -1)?1:(i + 1 < numberSteps);
		}
		if (arrayDistanceGen.size() != 0){
			std::string clusterGen = "";
			for (int i = 0; i < arrayDistanceGen.size(); ++i){
				clusterGen += '\t';
				clusterGen += gens[arrayDistanceGen[i].first];
				clusterGen += '\n';
			}
			genCluster.push_back(clusterGen);
		}
	}

};
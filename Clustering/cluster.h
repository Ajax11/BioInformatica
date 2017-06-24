
#include <iostream>
#include <vector>
#include <utility> //std::pair
#include <algorithm>    // std::swap
#include <math.h>
#include <fstream>
#include <string>
#include <map>

#include <ctime>
#include <ratio>
#include <chrono>

/*
	Making The cluster with a Matrix of Distance
*/


#define testNumGen 7
#define resourcePath "DataSet/"
#define resultPath "Resultados/"
#define link "Single Linkage"

struct cluster{
	
	cluster(){
		for (int i = 0; i < testNumGen; ++i){
			std::string gen = "";
			gen += '\t';
			gen += (int('A') + i); 
			gens.push_back( gen );
			gensCluster.insert( std::pair<int,std::string> (i, gen) );
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

		for (int i = 0; i < matrixOfDistance.size(); ++i){
			for (int j = i + 1; j < matrixOfDistance[i].size(); ++j){
				mapDistance.insert( std::pair< std::pair<int,int>, float > ( std::pair<int,int>(i,j) , matrixOfDistance[i][j] ) );
			}
		}
		clusterOfGens = gensCluster;
	}

	cluster(std::string file){
		std::ifstream stream( file.c_str() );
		std::string buffer;
		getline(stream, buffer , '\n');
		getline(stream, buffer , '\n');
		std::vector< std::vector<float> > vectorOfPointsOfTheGens;
		for (int i = 0; stream; ++i){
			std::string stringBuffer = "";
			stringBuffer += '\t';
			stringBuffer += buffer.substr(0,buffer.find('\t'));
			gens.push_back(stringBuffer);
			gensCluster.insert( std::pair<int,std::string> (i, stringBuffer) );
			buffer = buffer.substr(buffer.find('\t'));
			std::vector<float> pointsOfTheGens;
			while(buffer.size() > 1){
				std::string::size_type sizeOfTheNumber;
				float number = std::stof (buffer,&sizeOfTheNumber);
				buffer = buffer.substr(sizeOfTheNumber);
				pointsOfTheGens.push_back(number);
			}
			vectorOfPointsOfTheGens.push_back(pointsOfTheGens);
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
					for (int k = 0; k < vectorOfPointsOfTheGens[i].size(); ++k){
						float tmp = vectorOfPointsOfTheGens[j][k] - vectorOfPointsOfTheGens[i][k];
						distance += (tmp * tmp);
					}
					distance = sqrt(distance);
					matrixOfDistance[i][j] = distance;
					matrixOfDistance[j][i] = distance;
				}
			}
		}

		for (int i = 0; i < matrixOfDistance.size(); ++i){
			for (int j = i + 1; j < matrixOfDistance[i].size(); ++j){
				mapDistance.insert( std::pair< std::pair<int,int>, float > ( std::pair<int,int>(i,j) , matrixOfDistance[i][j] ) );
			}
		}
		clusterOfGens = gensCluster;
	}

	std::vector< std::string > gens;
	std::vector< std::vector<float> > matrixOfDistance;
	std::vector<std::string> genCluster;
	int numberSteps;

	std::map<int, std::string> gensCluster;
	std::map<int, std::string> clusterOfGens;
	std::map< std::pair<int,int> , float> mapDistance;

	void showCluster(){
		std::cout<<"The clusters are: "<<'\n';
		int i = 0;
		for ( std::map<int, std::string>::iterator it = clusterOfGens.begin(); it != clusterOfGens.end(); ++it , ++i){
			std::cout << "Cluster N° " << i << '\n' << it -> second << '\n';//<< " -> " << it -> first  
		}
		std::cout << '\n';
	}

	void saveCluster(std::string file = "", int steps = ){
		if (file.size() == 0){
			file += resultPath;
			file += "result-Jerarquico-Aglomerativo.txt";
		}

		std::ifstream stream( file );
		std::string buffer;
		getline(stream, buffer, '\n');
		stream.close();

		std::ofstream outputFile;
		outputFile.open( file.c_str() );
		outputFile << "Metodo: Clustering Jerarquico Aglomerativo " << link << '\n' << buffer << '\n' << '\n';

		outputFile << "Los clusters son: " << '\n' << '\n';
		for ( std::map<int, std::string>::iterator it = gensCluster.begin(); it != gensCluster.end(); ++it ){
			outputFile << "Cluster N° " << it -> first << '\n' << it->second << '\n';
		}
		outputFile.close();
	}

	void setGen(std::string gen = "A"){
		gens.push_back(gen);
	}

	void doCluster(string file = "", int steps = 1){
		auto begin = std::chrono::high_resolution_clock::now();
		makeCluster(steps);
		auto end = std::chrono::high_resolution_clock::now();
		showCluster();
		std::ofstream outputFile;
		if (file.size() == 0){
			file += resultPath;
			file += "result-Jerarquico-Aglomerativo.txt";
		}

		outputFile.open(file.c_str());
		outputFile << "Time:" << '\t';
		outputFile << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << " ns" <<'\n';
		outputFile.close();
		saveCluster(file);
	}

	void makeCluster(int steps = 1){

		std::pair< std::pair<int,int> ,float> maxPair;
		while(clusterOfGens.size() > steps){
			maxPair.first.first = clusterOfGens.begin() -> first;
			maxPair.first.second = (++(clusterOfGens.begin())) -> first;
			maxPair.second = mapDistance[ std::pair<int,int>( maxPair.first.first, maxPair.first.second) ];
			for (std::map<int, std::string>::iterator it = clusterOfGens.begin(); it != clusterOfGens.end(); ++it){
				std::map<int, std::string>::iterator itT = it;
				for (++itT; itT != clusterOfGens.end(); ++itT){
					if (maxPair.second > mapDistance[ std::pair<int,int>(it -> first, itT -> first) ]){
						maxPair.second = mapDistance[ std::pair<int,int>(it -> first, itT -> first) ];
						maxPair.first.first = it -> first;
						maxPair.first.second = itT -> first;
					}
				}
			}
			// En el caso de almacenar la distancia en la cual se une el cluster, aumentar un parametro a la estructura de almacenamiento
			std::string gen = "";
			gen += clusterOfGens[maxPair.first.first];
			gen += '\n';
			gen += clusterOfGens[maxPair.first.second];

			int position = gensCluster.size();
			/*
				Erasing the gens that are join
			*/
			clusterOfGens.erase(maxPair.first.first);
			clusterOfGens.erase(maxPair.first.second);
			/*
				Making the new Distances
			*/
			for (std::map<int, std::string>::iterator it = clusterOfGens.begin(); it != clusterOfGens.end(); ++it){
				/*
					Making the two pairs of distance to be compute
				*/
				std::pair<int,int> matchOne;
				std::pair<int,int> matchTwo;
				if (maxPair.first.first > it -> first){
					matchOne.first = it -> first;
					matchOne.second = maxPair.first.first;
				} else{
					matchOne.first = maxPair.first.first;
					matchOne.second = it -> first;
				}
				if (maxPair.first.second > it -> first){
					matchTwo.first = it -> first;
					matchTwo.second = maxPair.first.second;
				} else {
					matchTwo.first = maxPair.first.second;
					matchTwo.second = it -> first;
				}
				// Type of linkaged
				mapDistance.insert( std::pair< std::pair<int,int>, float > ( std::pair<int,int> (it -> first, position) , singleLink(matchOne, matchTwo) ) );
			}
			/*
				Insert The new Gen, that is the join of the selected gens
			*/
			clusterOfGens.insert (std::pair<int,std::string> (position, gen) );
			/*
				This Line is to store all the clusters that are made in the algorithm
			*/
			gensCluster.insert( std::pair<int,std::string> (position, gen) );
		}
	}

			
	inline float singleLink(std::pair<int,int> matchOne, std::pair<int,int> matchTwo){
		float distance = 0;
		// Single Link
		distance = (mapDistance[matchOne] > mapDistance[matchTwo])?mapDistance[matchTwo]:mapDistance[matchOne];
		return distance;
	}

	inline float completeLink(std::pair<int,int> matchOne, std::pair<int,int> matchTwo){
		float distance = 0;
		// complete Link
		distance = (mapDistance[matchOne] > mapDistance[matchTwo])?mapDistance[matchOne]:mapDistance[matchTwo];
		return distance;
	}

	inline float avarageLink(std::pair<int,int> matchOne, std::pair<int,int> matchTwo){
		float distance = 0;
		// complete - avarage Link
		distance = (mapDistance[matchTwo] + mapDistance[matchOne])/2.0;
		return distance;
	}


};




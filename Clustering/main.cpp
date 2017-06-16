#include <iostream>
//#include "disociativo.h"
#include "cluster.h"

using namespace std;

#define resourcePath "DataSet/"
#define resultPath "Resultados/"

int main(int argc, char const *argv[]){


	cluster singleLink;
	singleLink.makeCluster();
	singleLink.showCluster();


/*
	string file = "";
	file += resourcePath;
	file += "diauxic.txt";
	cluster disjing(file);

	cout<<"done Reading"<<'\n';

	disjing.doCluster();
	disjing.makeCluster();
	disjing.showCluster();
	disjing.saveCluster();
*/

	return 0;
}
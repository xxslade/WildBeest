#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <limits>

using namespace std;

// Function to return a random number within a range
double randomDouble(double min, double max) {
    // Generate a random integer between 0 and RAND_MAX
    int randomInt = rand();
    // Map it to the range [min, max]
    return min + (static_cast<double>(randomInt) / RAND_MAX) * (max - min);
}

// Function to return random x and y, i.e., a random position
pair<double, double> randomPos(double min, double max) {
    return {randomDouble(min, max), randomDouble(min, max)};
}

//Test Function to test our optimisation
double testFunction(pair<double,double> position){
	double x = position.first;
	double y = position.second;
	
	//Rastrigin's Function
	double rast = 20 + x*x + y*y - 10 * (cos(2 * M_PI * x) + cos(2 * M_PI * y));
	return rast;
}

int findLeaderInd(vector<double> functionValue){
	int size = functionValue.size();
	double minFunc = numeric_limits<double>::max();
	int minIndex = -1;
	for(int i=0; i<size; i++){
		if(functionValue[i]<minFunc){
			minFunc = functionValue[i];
			minIndex = i;
		}
	}
	return minIndex;
}

// Number of Positions
const int noOfPositions = 30; 
// Maximum number of iterations
const int iterations = 10000;   
// Controls predator pressure (0.8 = 80% herd movement) 
const double predator_factor = 0.8; 
// Search space bounds
const double min_bound = -2, max_bound = 2; 
// Step size towards leader
const double move_step = 0.05;  

//Main Function
int main(){

	srand(static_cast<unsigned>(time(0)));
	//Generating all the random positions and their corresponding Function values
	
	vector<pair<double, double>> positions (noOfPositions); //initialising the positions vector
	vector<double> functionValue (noOfPositions);			//initialising the functions_value vector
	
	for (int i=0; i<noOfPositions; i++){
		double randX = randomDouble(min_bound, max_bound);
		double randY = randomDouble(min_bound, max_bound);
		pair<double, double> pos = {randX, randY};
		positions[i] = pos;								// assigning the value to ith position
		functionValue[i] = testFunction(positions[i]);	// assinging the value to ith function
	}
	
	//Finding the leader position (the value of the position whose correspoding funtion value is the least, since we want to minimise the function)
	double minFunc = numeric_limits<double>::max();
	int leaderIndex = -1;
	for (int i=0; i<noOfPositions; i++){
		if (functionValue[i]<minFunc){
			minFunc = functionValue[i];
			leaderIndex = i;
		}
	}
	cout<<functionValue[leaderIndex]<<endl;
	pair<double, double> leader = positions[leaderIndex];  //we got the position of the leader
	for (int it = 0; it<iterations; it++){
		
		for (int i = 0; i<noOfPositions; i++){
			//ensure some positions get randomly initialised. (to escape local mimima)
			if (randomDouble(0.0, 1.0) < predator_factor || i==leaderIndex){   //i == leaderIndex so that it doesn't get reinitialised random value which will result in loosing of leader)
				// update the position, move towards leader
				positions[i].first = positions[i].first + (leader.first - positions[i].first) * randomDouble(0.0, move_step);     //update the x coordinate
				positions[i].second = positions[i].second + (leader.second - positions[i].second) * randomDouble(0.0, move_step); //update the y coordinate
				//cout<<functionValue[leaderIndex]<<endl;
			}
			else{
				positions[i] = randomPos(min_bound, max_bound);
			}
			// ensuring the positions remain in bounds after updating the positons (i.e., when they move towards leader)
			if (positions[i].first > max_bound){
				positions[i].first = max_bound;		//x coordinate below upper bound
			}
			if (positions[i].first < min_bound){
				positions[i].first = min_bound;		//x coordinate above lower bound
			}
			if (positions[i].second > max_bound){	
				positions[i].second = max_bound;		//y coordinate below upper bound
			}
			if (positions[i].second < min_bound){
				positions[i].second = min_bound;		//y coordinate above lower bound
			}
			
			//updating their corresponding function value
			functionValue[i] = testFunction(positions[i]);
				
		}
		//cout<<functionValue[leaderIndex]<<endl;
		
		// check if we have a new leader. (a new lowest functionValue)
		int newLeaderInd = findLeaderInd(functionValue);
		if (functionValue[newLeaderInd] < functionValue[leaderIndex]){
			leaderIndex = newLeaderInd;
			leader = positions[newLeaderInd];
		}
		if(it%100 == 0)
		cout<<functionValue[leaderIndex]<<endl;
		//cout<<functionValue[leaderIndex]<<endl;
	}
	//cout<<functionValue[leaderIndex];
	
	return 0;
	
	
	
	
}

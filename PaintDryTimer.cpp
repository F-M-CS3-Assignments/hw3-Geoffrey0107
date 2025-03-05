#include <ctime> // for time(0)
#include <iostream> // for cin and cout
#include <cmath> // for M_PI and others
#include <vector> // for vectors (duh)
#include <cstdlib> // for random
#include <cassert> // for assert in the tests() function
#include "TimeCode.h" // for timecode's (duh)

using namespace std;

struct DryingSnapShot {
	// This is a struct, it's like an object
	// that doesn't have any methods.
	// You can read more about them in the ZyBook
	// just search for "struct"
	string name;
	time_t startTime;
	TimeCode *timeToDry;
};

long long int get_time_remaining(DryingSnapShot dss){
	int secondsElapsed = time(0) - dss.startTime;
	TimeCode timeElapsed = TimeCode(0,0,secondsElapsed);
	//dereference a pointer before performing operations such as "-"
	if(*(dss.timeToDry) < timeElapsed){//Time remaining less than 0
		TimeCode timeRemaining(0,0,0);
		return timeRemaining.GetSeconds();
	}else{//returns the total seconds of the TimeCode object
		TimeCode timeRemaining = *(dss.timeToDry) - timeElapsed;
		return timeRemaining.ComponentsToSeconds(timeRemaining.GetHours(),timeRemaining.GetMinutes(),timeRemaining.GetSeconds());
	}
}

//(takes 0:44:9 to dry) time remaining: 0:43:43
string drying_snap_shot_to_string(DryingSnapShot dss){
	string result;
	TimeCode remaining = TimeCode(0,0,get_time_remaining(dss));
	if(remaining.GetSeconds()== 0){// if the remaining time is 0, remaining time not included in result 
		result = "(takes " + dss.timeToDry->ToString() + " to dry)";
	}
	else{// if the remaining time is larger than 0, remaining time is included
		result = "(takes " + dss.timeToDry->ToString() + " to dry) time remaining: " + remaining.ToString();
	}
	return result;
}


double get_sphere_sa(double rad){
	return 4*M_PI*rad*rad;
}


TimeCode *compute_time_code(double surfaceArea){
	int timeNeeded = static_cast<int>(surfaceArea);
	return new TimeCode(0,0,timeNeeded);
}


void tests(){
	// get_time_remaining
	DryingSnapShot dss;
	dss.name = "ball";
	dss.startTime = time(0);
	TimeCode tc = TimeCode(0, 0, 7);
	dss.timeToDry = &tc;
	long long int ans = get_time_remaining(dss);
	cout << "Time remaining: " << ans << endl;
	assert(ans > 6 && ans < 8);
	
	cout << drying_snap_shot_to_string(dss) << endl;


	// get_sphere_sa
	double sa = get_sphere_sa(2.0);
	assert (50.2654 < sa && sa < 50.2655);
	cout << "compute time sa" << compute_time_code(sa)->ToString() << endl;


	// compute_time_code
	TimeCode *tc2 = compute_time_code(1.0);
	//cout << "tc: " << tc.GetTimeCodeAsSeconds() << endl;
	assert(tc2->GetTimeCodeAsSeconds() == 1);
	delete tc2;

    // Test sphere surface area with zero radius
    double saZero = get_sphere_sa(0.0);
    assert(saZero == 0.0);

    // no remaining time
    DryingSnapShot dss2;
    dss2.name = "instant-dry";
    dss2.startTime = time(0) - 5; 
    dss2.timeToDry = new TimeCode(0, 0, 5);
    long long remaining = get_time_remaining(dss2);
    cout << "Remaining for instant-dry is " << remaining << endl;
    assert(remaining <= 0);
    delete dss2.timeToDry;

    // Test large radius
    double bigRadius = 100.0;
    double saBig = get_sphere_sa(bigRadius);
    cout << "Big sphere SA: " << saBig << endl;
    assert(saBig > 125000 && saBig < 126000);
    TimeCode *tcBig = compute_time_code(saBig);
    cout << "Time needed for big sphere: " << tcBig->ToString() << endl;
    delete tcBig;

    // Test a small radius
    double smallRad = 1.0;
    double smallSa = get_sphere_sa(smallRad);
    assert(smallSa > 12.56 && smallSa < 12.57); 
    TimeCode *tcSmall = compute_time_code(smallSa);
    assert(tcSmall->GetTimeCodeAsSeconds() == 12);
    delete tcSmall;

    // Test batch removal
    vector<DryingSnapShot> batch;
    DryingSnapShot testBatch;
    testBatch.name = "test-batch";
    testBatch.startTime = time(0) - 10; // 10 seconds ago
    testBatch.timeToDry = new TimeCode(0, 0, 8); // Dries in 8 seconds
    batch.push_back(testBatch);
    
    cout << "Before cleanup: " << batch.size() << " batches" << endl;
    for (size_t i = 0; i < batch.size(); ) {
        if (get_time_remaining(batch[i]) <= 0) {
            delete batch[i].timeToDry;
            batch.erase(batch.begin() + i);
        } else {
            i++;
        }
    }
    cout << "After cleanup: " << batch.size() << " batches" << endl;
    assert(batch.size() == 0);

    cout << "ALL TESTS PASSED!" << endl;
}


int main(){
	vector<DryingSnapShot> batch;
	string input;
	double rad;

	tests();
	while(true){
		cout << "Choose an option: (A)dd, (V)iew Current Items, (Q)uit: ";
		cin >> input;
		if(input == "a" || input == "A"){
			cout << "Enter radius: ";
			//here to need to check if user enters a number not a character or string 
			//https://stackoverflow.com/questions/27003967/how-to-check-if-cin-is-int-in-c
			if (!(cin >> rad)) { // If input is not a number
				cin.clear(); // Clear error state
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Remove invalid input from buffer
				cout << "Radius must be a number!" << endl;
			} else {// initialize a new DryingSnapShot object 
				DryingSnapShot newBatch; 
				newBatch.name = "Batch-" + to_string(rand());// the name is a random number
				newBatch.startTime = time(0);
				newBatch.timeToDry = compute_time_code(get_sphere_sa(rad));
            	batch.push_back(newBatch);
			}
		}
		else if(input == "v" || input == "V"){
			for (size_t i = 0; i < batch.size();) {// cannot compare int i to batchsize which return size_t
				long long remainingTime = get_time_remaining(batch[i]);
				if(remainingTime > 0){
					cout << batch[i].name << drying_snap_shot_to_string(batch[i]) << endl;
					i++;// only increment i when nothing is removed, or elements are skipped. 
				}
				else{// when batch time reaches 0, remove the batch from the vector. 
					cout << batch[i].name << drying_snap_shot_to_string(batch[i]) << " DONE " << endl;
					delete batch[i].timeToDry;
        			batch.erase(batch.begin() + i);
				}
            }
			cout << batch.size() << " batches being tracked." << endl;
		}
		else if(input == "Q" || input == "q"){
			return false;
		}
	}
	
    return 0;
}
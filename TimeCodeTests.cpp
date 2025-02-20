#include <iostream>
#include <assert.h>
using namespace std;

#include "TimeCode.h"

void TestComponentsToSeconds(){
	cout << "Testing ComponentsToSeconds" << endl;
	
	// Random but "safe" inputs
	long long unsigned int t = TimeCode::ComponentsToSeconds(3, 17, 42);
	assert(t == 11862);

    // Test Case 3: One hour
    long long unsigned int t3 = TimeCode::ComponentsToSeconds(1, 0, 0);
    assert(t3 == 3600);  // Expected: 3600 seconds

    // Test Case 4: One minute
    long long unsigned int t4 = TimeCode::ComponentsToSeconds(0, 1, 0);
    assert(t4 == 60);  // Expected: 60 seconds

    // Test Case 5: One second
    long long unsigned int t5 = TimeCode::ComponentsToSeconds(0, 0, 1);
    assert(t5 == 1);  // Expected: 1 second

    // Test Case 6: Edge case 
    long long unsigned int t6 = TimeCode::ComponentsToSeconds(0, 59, 59);
    assert(t6 == 3599);  

    // Test Case 7: Large hour to seconds
    long long unsigned int t7 = TimeCode::ComponentsToSeconds(99, 0, 0);
    assert(t7 == 356400);  

	// Test Case 2: Zero time
    long long unsigned int t2 = TimeCode::ComponentsToSeconds(0, 0, 0);
    assert(t2 == 0); 

	
	cout << "PASSED!" << endl << endl;
}


void TestDefaultConstructor(){
	cout << "Testing Default Constructor" << endl;
	TimeCode tc;
	
	//cout << "Testing ToString()" << endl;
	//cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	cout << "PASSED!" << endl << endl;
}


void TestComponentConstructor(){
	cout << "Testing Component Constructor" << endl;
	TimeCode tc = TimeCode(0, 0, 0);
	cout << "Testing ToString()" << endl;
	cout << "tc: " << tc.ToString() << endl;
	assert(tc.ToString() == "0:0:0");
	
	// Test Case 2: Overflow Test
    TimeCode tc4 = TimeCode(99, 99, 99);
    cout << "tc4: " << tc4.ToString() << endl;
    assert(tc4.ToString() == "100:40:39");

	// Roll-over inputs
	TimeCode tc3 = TimeCode(3, 71, 3801);
	cout << "tc3: " << tc3.ToString() << endl;
	assert(tc3.ToString() == "5:14:21");
	
	cout << "PASSED!" << endl << endl;
}


void TestGetComponents(){
	cout << "Testing GetComponents" << endl;
	
	unsigned int h;
	unsigned int m;
	unsigned int s;
	
	// Test Case 1: 
	TimeCode tc1 = TimeCode(5, 2, 18);
	tc1.GetComponents(h, m, s);
	assert(h == 5 && m == 2 && s == 18);
	
	// Test Case 2: 
	TimeCode tc2 = TimeCode(0, 0, 0);
	tc2.GetComponents(h, m, s);
	assert(h == 0 && m == 0 && s == 0);
	
	// Test Case 3: roll over cases
	TimeCode tc3 = TimeCode(99, 99, 99);
	tc3.GetComponents(h, m, s);
	assert(h == 100 && m == 40 && s == 39);
	
	cout << "PASSED!" << endl << endl;
}


void TestAddition() {
    cout << "Testing Addition Operator +" << endl;
    
    // Test Case 1: Simple Addition
    TimeCode tc1(10, 30, 30);
    TimeCode tc2(15, 35, 40);
    TimeCode tc3 = tc1 + tc2;
    assert(tc3.ToString() == "26:6:10");
	cout << "Test Case 1 Passed!" << endl;

	//Test Case 2: adding a 0 time should not change the time
	TimeCode tc4 = (0,0,0);
	TimeCode tc5 = tc1 + tc4;
	assert(tc5.ToString() == tc1.ToString());
	cout << "Test Case 2 Passed!" << endl;

    cout << "Addition Operator Test PASSED!" << endl << endl;
}

void TestSubtract(){
	cout << "Testing Subtraction Operator -" << endl;

	// Test Case 1: Normal subtraction
	TimeCode tc1 = TimeCode(1, 0, 0);
	TimeCode tc2 = TimeCode(0, 50, 0);
	TimeCode tc3 = tc1 - tc2;
	assert(tc3.ToString() == "0:10:0");
	cout << "Test Case 1 Passed!" << endl;
	
	// Test Case 2: Handling Negative Time (Should throw an error)
	TimeCode tc4 = TimeCode(1, 15, 45);
	try {
		TimeCode tc5 = tc1 - tc4;
		// If no exception is thrown, fail
		assert(false);
	} catch (const invalid_argument& e) {
		cout << "Caught expected error: " << e.what() << endl;
		assert(true);  
	}
	cout << "Test Case 2 Passed!" << endl;

	cout << "Subtraction Operator Test PASSED!" << endl << endl;
}

void TestMultiplication() {  
	cout << "Testing Multiplication Operator *" << endl;

    // Test Case 1: Simple Multiplication
    TimeCode tc1(10, 30, 30);
    double a = 2;
    TimeCode tc3 = tc1 * a;
    assert(tc3.ToString() == "21:1:0");
	cout << "Test Case 1 Passed!" << endl;

	// Test Case 2: Multiply by 0
	double b = 0;
	TimeCode tc4 = tc1 * b;
	assert(tc4.ToString() == "0:0:0");
	cout << "Test Case 2 Passed!" << endl;

	// Test Case 3: Multiply by negative number
	double c = -2;
	try{
	TimeCode tc5 = tc1 * c;
	assert(false);
	} catch (const invalid_argument& e) {
		cout << "Caught expected error: " << e.what() << endl;
		assert(true);  
	}
	cout << "Test Case 3 Passed!" << endl;

    cout << "Multiplication Operator Test PASSED!" << endl << endl;
}

void TestDivision() {
	cout << "Testing Division Operator /" << endl;

    // Test Case 1: Simple Division
    TimeCode tc1(10, 30, 30);
    double a = 2;
    TimeCode tc3 = tc1 / a;
    assert(tc3.ToString() == "5:15:15");
	cout << "Test Case 1 Passed!" << endl;

	//Test Case 2: Large number Division
	double b = 100;
	TimeCode tc5 = tc1 / b;
    assert(tc5.ToString() == "0:6:18");
	cout << "Test Case 2 Passed!" << endl;

	//Test Case 3: Super large number Division, when result is smaller than 1 second.
	double c = 1000000;
	TimeCode tc7 = tc1 / c;
    assert(tc7.ToString() == "0:0:0");
	cout << "Test Case 3 Passed!" << endl;

    cout << "Division  Operator Test PASSED!" << endl << endl;
}

void TestEqualityOperator() {
    cout << "Testing Equality Operator ==" << endl;

    // Test Case 1: same time
    TimeCode tc1(10, 30, 30);
    TimeCode tc2(10, 30, 30);
    assert(tc1 == tc2);
    cout << "Test Case 1 Passed!" << endl;

    // Test Case 2: different time
    TimeCode tc3(10, 30, 31); 
    assert(!(tc1 == tc3));
    cout << "Test Case 2 Passed!" << endl;

    // Test Case 3: different hour
    TimeCode tc4(11, 30, 30);
    assert(!(tc1 == tc4));
    cout << "Test Case 3 Passed!" << endl;

    // Test Case 4: different minute
    TimeCode tc5(10, 31, 30);
    assert(!(tc1 == tc5));
    cout << "Test Case 4 Passed!" << endl;

	// Test Case 5: different time should not be equal
	TimeCode tc6(0, 0, 0);
	assert(!(tc6 == tc5));
	cout << "Test Case 5 Passed!" << endl;



    cout << "Equality Operator Test PASSED!" << endl << endl;
}

void TestInequalityOperator() {
    cout << "Testing Inequality Operator !=" << endl;

    // Test Case 1: same time
    TimeCode tc1(10, 30, 30);
    TimeCode tc2(10, 30, 30);
    assert(!(tc1 != tc2));
    cout << "Test Case 1 Passed!" << endl;

    // Test Case 2: different time
    TimeCode tc3(10, 30, 31); 
    assert(tc1 != tc3);
    cout << "Test Case 2 Passed!" << endl;

    // Test Case 3: different hour
    TimeCode tc4(11, 30, 30);
    assert(tc1 != tc4);
    cout << "Test Case 3 Passed!" << endl;

    // Test Case 4: different minute
    TimeCode tc5(10, 31, 30);
    assert(tc1 != tc5);
    cout << "Test Case 4 Passed!" << endl;

    cout << "Inequality Operator Test PASSED!" << endl << endl;
}

void TestComparisonOperator() {
    cout << "Testing Comparison Operator ==" << endl;

    // Test Case 1: test <
    TimeCode tc1(10, 10, 10);
    TimeCode tc2(10, 30, 30);
    assert(tc1 < tc2);
    cout << "Test Case 1 Passed!" << endl;

    // Test Case 2: test >
    TimeCode tc3(10, 30, 31); 
    assert(tc3 >tc1);
    cout << "Test Case 2 Passed!" << endl;

    // Test Case 3: test <=
    TimeCode tc4(10, 30, 30);
    assert(tc4 <= tc3 && tc4 <=tc2);
    cout << "Test Case 3 Passed!" << endl;

    // Test Case 4: test >=
    assert(tc3 >= tc4 && tc4 >= tc2);
    cout << "Test Case 4 Passed!" << endl;

    cout << "Comparison Operator Test PASSED!" << endl << endl;
}

void TestSetMinutes()
{
	cout << "Testing SetMinutes" << endl;

	TimeCode tc = TimeCode(8, 5, 9);
	tc.SetMinutes(15); // test valid change
	assert(tc.ToString() == "8:15:9");

	try
	{
		tc.SetMinutes(80);  // test invalid change
		assert(false);
	}
	catch (const invalid_argument &e)
	{
		// cout << e.what() << endl;
	}

	assert(tc.ToString() == "8:15:9");

	cout << "PASSED!" << endl << endl;
}

void TestResetFunction() {
    cout << "Testing Reset" << endl;
    
    TimeCode tc(10, 30, 30); 
    assert(tc.ToString() == "10:30:30");
    tc.reset(); 
    assert(tc.ToString() == "0:0:0");

    cout << "Reset Function Test PASSED!" << endl << endl;
}
	
int main(){
	TestComponentsToSeconds();
	TestDefaultConstructor();
	TestComponentConstructor();
	TestGetComponents();
	TestAddition();
	TestSubtract();
	TestMultiplication();
	TestDivision();
	TestEqualityOperator();
	TestInequalityOperator();
	TestComparisonOperator();
	TestResetFunction();
	
	cout << "PASSED ALL TESTS!!!" << endl;
	return 0;
}

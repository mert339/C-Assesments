#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <clocale>
#include <iomanip>

////////////-----------040190737 MERT OLPAK HW2------------//////////


using namespace std;
int number_of_cities = 82;
int key = 0;
int state_total, foundation_total, mul_state, mul_foundation, one_state, one_foundation = 0;
map<int, string> city; //definig a map that will match city codes with city names as requested


class University
{ 
 private:
    string university_name;
    int city_code;
    char type;
 
 public:
    University(string uni_name , int ct_code, char typ){
        university_name = uni_name;
        city_code = ct_code;
        type = typ;
    }
    virtual void print(){
        key = city_code;
        auto iter = city.find(key);
        cout << left << setw(40) << university_name << " " << left << setw(20) << iter -> second << " ";
        if (type == 'S'){cout << left << setw(12) << "State "; state_total++;} 
        else if (type == 'F') {cout << setw(12) << "Foundation ";foundation_total++;}                
    }
};

class MultiCampusUniversity : public University
{
 private:
    int number_of_campuses;

 public:
    MultiCampusUniversity(string uni_name, int ct_code, char typ, int num_of_campuses):University(uni_name, ct_code, typ){
        number_of_campuses = num_of_campuses;
        if (typ == 'S'){mul_state++;} else if (typ == 'F') {mul_foundation++;}
    }
    void print (){ //Specialized print function that will bypass base print function when called
        University :: print (); 
        cout << setw(10) << right << number_of_campuses << " ";
    }
};


    
int main() {

    setlocale(LC_ALL, "Turkish"); //Setting library for turkish characters
    string line;
    
    ifstream inputFile("cities.txt"); //Opening the txt file in reading mode
    
    if (!inputFile) { //Checking for errors while opening
        cerr << "Failed to open the file." << endl;
        return 1;
    }
    string value;

    for (int i = 0; i < number_of_cities; ++i) { 
        if (getline(inputFile, line)) {
            // Extract the key and value from the line
            istringstream iss(line);
            if (iss >> key >> value) {
                // Insert the key/value pair into the map
                city[key] = value;
            }
        }
    }

    // Print the key/value pairs stored in the map
    /*for (const auto& pair : city) {
        cout << "Key: " << pair.first << ", Value: " << pair.second << endl;
    }*/

    inputFile.close();
 
 
 ////////////////////////////////////////////////////////////////////////////////////////////////

    vector<University*> universities;  //defining a vector for unniversities as requested
    vector<University*>::const_iterator it; //defining an iterator to find stuff in vector

    ifstream file("universities.txt"); //Opening the txt file in reading mode

    while (getline(file, line)) { //Getting info from every single line until incoming data is null
        istringstream iss(line); //read to the previously defined string "line"
        string name;
        int cityCode;
        char universityType;
        int numOfCampuses = -1;  // Default value for lines without numOfCampuses parameter

        if (getline(iss, name, ';') &&
            iss >> cityCode &&
            iss.ignore(1) >> universityType) {
            if (iss.peek() == ';') {
                iss.ignore(1);
                iss >> numOfCampuses;
            }

            if (numOfCampuses != -1) { //if numOfCampuses parameter is changed, then it is a derived class
                universities.push_back(new MultiCampusUniversity(name, cityCode, universityType, numOfCampuses));
                cout << endl;
            } else { //if numOfCampuses parameter is not changed, then it is a base class
                universities.push_back(new University(name, cityCode, universityType));
                cout << endl;
            }
        } else {
            cerr << "Error parsing line: " << line << endl;
        }
    }

    file.close();

    int multi_total = 0; //number of multicampus universities
    int one_total = 0; //number of 1-campus universities
    
    cout << "ONE-CAMPUS UNIVERSITIES :" << endl << left << setw(45) << "    University Name" << left << setw(21) << "City" << left << setw(12) << "Type" << endl;

    for (it = universities.begin(); it != universities.end(); ++it) { //print the derived universities vector members untill end of it using the previously defined iterator
            
        if (dynamic_cast<MultiCampusUniversity*>(*it) == nullptr) {
            // Object is not of type DerivedUniversity (i.e., it is of type University or other derived class)
            cout << right << setw(3) << one_total+1 << " ";
            (*it) -> print ();
            cout << endl;
            one_total++;
        }
    }
    
    cout << endl << endl << endl << endl << endl;
    
    cout << "MULTI-CAMPUS UNIVERSITIES :" << endl << left << setw(45) << "    University Name" << left << setw(21) << "City" << left 
    << setw(18) << "Type" << left << setw(8) << "Campuses" << endl;
    for (it = universities.begin(); it != universities.end(); ++it) { //print the base universities vector members untill end of it using the previously defined iterator
        auto iter = city.find(key);    
        if (MultiCampusUniversity* derivedObj = dynamic_cast<MultiCampusUniversity*>(*it)) {
            // Object is of type DerivedUniversity
            cout << right << setw(3) << multi_total+1 << " ";
            (*it) -> print ();
            cout << endl;
            multi_total++;
        }     
    }
    

    std::setlocale(LC_ALL, ".437"); //setting library again in order to print rectangles
    one_state = state_total - mul_state; //calculating number of one-campus state universities 
    one_foundation = foundation_total - mul_foundation; //calculating the number of one-campus foundatiton universities

    cout << endl << "HISTOGRAMS OF TOTAL UNIVERSITY NUMBERS:" << endl << endl;
    
    cout << left << setw(27) << "ONE CAMPUS (STATE)"; cout << ": " << left << setw(2) << one_state << " "; 
    for (int i = 0; i < one_state; i++){cout << char(254);}
    cout << endl;
    
    cout << left << setw(27) << "ONE CAMPUS (FOUNDATION)"; cout << ": " << left << setw(2) << one_foundation << " "; 
    for (int i = 0; i < one_foundation; i++){cout << char(254);}
    cout << endl;

    cout << left << setw(27) << "ONE CAMPUS TOTAL"; cout << ": " << left << setw(2) << one_total << " "; 
    for (int i = 0; i < one_total; i++){cout << char(254);}
    cout << endl << endl;

    cout << left << setw(27) << "MULTI CAMPUS (STATE))"; cout << ": " << left << setw(2) << mul_state << " "; 
    for (int i = 0; i < mul_state; i++){cout << char(254);}
    cout << endl;

    cout << left << setw(27) << "MULTI CAMPUS (FOUNDATION)"; cout << ": " << left << setw(2) << mul_foundation << " "; 
    for (int i = 0; i < mul_foundation; i++){cout << char(254);}
    cout << endl;

    cout << left << setw(27) << "MULTI CAMPUS TOTAL"; cout << ": " << left << setw(2) << multi_total << " "; 
    for (int i = 0; i < multi_total; i++){cout << char(254);}
    cout << endl << endl;

    cout << left << setw(27) << "OVERALL TOTAL (STATE)"; cout << ": " << left << setw(2) << state_total << " "; 
    for (int i = 0; i < state_total; i++){cout << char(254);}
    cout << endl;

    cout << left << setw(27) << "OVERALL TOTAL (FOUNDATITON)"; cout << ": " << left << setw(2) << foundation_total << " "; 
    for (int i = 0; i < foundation_total; i++){cout << char(254);}
    cout << endl << endl;

    cout << "PROGRAM FINISHED." << endl;

    system("pause");
    
    return 0;
}
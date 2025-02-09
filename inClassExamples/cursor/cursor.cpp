#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // for setprecision
#include <limits>  // for numeric_limits

using namespace std;

class Student {
private:
    string name;
    vector<double> grades;
    
public:
    // Constructor
    Student(string studentName) : name(studentName) {}
    
    // Methods
    void addGrade(double grade) {
        if (grade >= 0 && grade <= 100) {
            grades.push_back(grade);
        }
    }
    
    double getAverage() const {
        if (grades.empty()) return 0.0;
        double sum = 0;
        for (double grade : grades) {
            sum += grade;
        }
        return sum / grades.size();
    }
    
    string getName() const { return name; }
    
    void displayGrades() const {
        cout << "Grades for " << name << ": ";
        for (double grade : grades) {
            cout << grade << " ";
        }
        cout << "\nAverage: " << fixed << setprecision(2) << getAverage() << endl;
    }
};

class GradeManager {
private:
    vector<Student> students;
    
public:
    void addStudent() {
        string name;
        cout << "Enter student name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, name);
        
        students.push_back(Student(name));
        cout << "Student added successfully!\n";
    }
    
    void addGrade() {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        displayStudentList();
        int index;
        double grade;
        
        cout << "Enter student number: ";
        cin >> index;
        
        if (index < 1 || index > students.size()) {
            cout << "Invalid student number!\n";
            return;
        }
        
        cout << "Enter grade (0-100): ";
        cin >> grade;
        
        students[index-1].addGrade(grade);
        cout << "Grade added successfully!\n";
    }
    
    void displayStudentList() const {
        cout << "\nStudent List:\n";
        for (size_t i = 0; i < students.size(); i++) {
            cout << i + 1 << ". " << students[i].getName() << endl;
        }
    }
    
    void displayAllGrades() const {
        if (students.empty()) {
            cout << "No students registered yet!\n";
            return;
        }
        
        cout << "\n=== Grade Report ===\n";
        for (const Student& student : students) {
            student.displayGrades();
            cout << "-------------------\n";
        }
    }
};

int main() {
    GradeManager manager;
    int choice;
    
    while (true) {
        cout << "\n=== Student Grade Manager ===\n"
             << "1. Add Student\n"
             << "2. Add Grade\n"
             << "3. Display All Grades\n"
             << "4. Exit\n"
             << "Enter your choice: ";
        
        cin >> choice;
        
        switch (choice) {
            case 1:
                manager.addStudent();
                break;
            case 2:
                manager.addGrade();
                break;
            case 3:
                manager.displayAllGrades();
                break;
            case 4:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
    
    return 0;
}
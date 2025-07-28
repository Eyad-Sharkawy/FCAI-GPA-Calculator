#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
#include <limits>
#include <cctype>
#include <fstream>


const std::unordered_map<std::string, double> POINTS_MAP = {
    {"A+", 4.0}, {"A", 3.7}, {"A-", 3.4}, {"B+", 3.2}, {"B", 3.0}, {"B-", 2.8},
    {"C+", 2.6}, {"C", 2.4}, {"C-", 2.2}, {"D+", 2.0}, {"D", 1.5}, {"D-", 1.0}, {"F", 0.0}
};

const std::unordered_map<double, std::string> GRADE_MAP = {
    {4.0, "A+"}, {3.7, "A"}, {3.4, "A-"}, {3.2, "B+"}, {3.0, "B"}, {2.8, "B-"},
    {2.6, "C+"}, {2.4, "C"}, {2.2, "C-"}, {2.0, "D+"}, {1.5, "D"}, {1.0, "D-"}, {0.0, "F"}
};

struct Course {
    std::string name;
    int credits;
    double points;
};

void deleteCourse(std::vector<Course>& courses);
void saveCourses(const std::vector<Course>& courses, const std::string& filename);
void loadCourses(std::vector<Course>& courses, int& unnamedCounter, const std::string& filename);
void printMenu(const std::vector<Course>& courses);
Course getCourseDetailsFromUser(int& unnamedCounter);
void addCourse(std::vector<Course>& courses, int& unnamedCounter);
void viewCourses(const std::vector<Course>& courses);
void editCourse(std::vector<Course>& courses, int& unnamedCounter);
double calculateGPA(const std::vector<Course>& courses);
int totalHours(const std::vector<Course>& courses);

int main() {
    std::vector<Course> courses;
    int unnamedCoursesCounter = 0;
    int option;
    const std::string FILENAME = "gpa_data.txt";

    loadCourses(courses, unnamedCoursesCounter, FILENAME);

    while (true) {
        printMenu(courses);
        std::cout << "Enter your choice: ";
        std::cin >> option;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (option) {
            case 1:
                addCourse(courses, unnamedCoursesCounter);
                break;
            case 2:
                viewCourses(courses);
                break;
            case 3:
                if (!courses.empty()) {
                    editCourse(courses, unnamedCoursesCounter);
                } else {
                    std::cout << "No courses to edit.\n";
                }
                break;
            case 4:
                if (!courses.empty()) {
                    deleteCourse(courses);
                } else {
                    std::cout << "No courses to delete.\n";
                }
                break;
            case 5:
                while (true) {
                    std::cout << "Do you want to save your progress? (Yes | No)\n";
                    std::string response;
                    std::cin >> response;
                    for (auto& c : response) { c = tolower(c); }
                    if (response == "yes" || response == "no") {
                        if (response == "yes") {
                            saveCourses(courses, FILENAME);
                            std::cout << "Exiting. Goodbye!\n";
                            return 0;
                        }
                        else {
                            std::cout << "Exiting. Goodbye!\n";
                            return 0;
                        }
                    }
                    std::cout << "Invalid option! Please try again. \n";
                }
            default:
                std::cout << "Invalid option! Please try again.\n";
                break;
        }
        std::cout << "\n";
    }
}

void deleteCourse(std::vector<Course>& courses) {
    viewCourses(courses);
    int choice = 0;

    while (true) {
        std::cout << "Enter the number of the course to edit: ";
        std::cin >> choice;
        if (!std::cin.fail() && choice > 0 && choice <= courses.size()) {
            break;
        }
        std::cout << "Invalid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    courses.erase(courses.begin() + --choice);
    std::cout << "Course updated deleted!\n";
}

void saveCourses(const std::vector<Course>& courses, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for saving: " << filename << std::endl;
        return;
    }

    for (const auto& course : courses) {
        outFile << course.name << '\n';
        outFile << course.credits << '\n';
        outFile << course.points << '\n';
    }
    outFile.close();
    std::cout << "Progress saved successfully.\n";
}

void loadCourses(std::vector<Course>& courses, int& unnamedCounter, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        Course course;
        course.name = line;

        if (std::getline(inFile, line)) {
            try { course.credits = std::stoi(line); }
            catch (const std::exception& e) { continue;}
        } else break;

        if (std::getline(inFile, line)) {
            try { course.points = std::stod(line); }
            catch (const std::exception& e) { continue;}
        } else break;

        if (course.name.rfind("Unnamed Course ", 0) == 0) {
            try {
                int num = std::stoi(course.name.substr(15));
                if (num > unnamedCounter) {
                    unnamedCounter = num;
                }
            } catch (const std::exception& e) {}
        }
        courses.push_back(course);
    }
    inFile.close();
}

void printMenu(const std::vector<Course>& courses) {
    std::cout << "==============================================" << std::endl;
    std::cout << "================GPA Calculator================" << std::endl;
    std::cout << "==============================================" << std::endl;

    if (!courses.empty()) {
        std::cout.precision(3);
        std::cout << "Current GPA: " << calculateGPA(courses) << "\n";
        std::cout << "Total completed hours: " << totalHours(courses) << "\n";
        std::cout << "----------------------------------------------\n";
    }

    std::cout << "1. Add a Course" << std::endl;
    std::cout << "2. View Courses" << std::endl;
    std::cout << "3. Edit Courses" << std::endl;
    std::cout << "4. Delete a Course" << std::endl;
    std::cout << "5. Exit" << std::endl;
}

Course getCourseDetailsFromUser(int& unnamedCounter) {
    Course course;
    std::string tempString;

    std::cout << "Course Name (Optional): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, course.name);
    if (course.name == "0" || course.name.empty()) {
        course.name = "Unnamed Course " + std::to_string(++unnamedCounter);
    }

    while (true) {
        std::cout << "Course Credits (0-9): ";
        std::cin >> course.credits;
        if (!std::cin.fail() && course.credits >= 0 && course.credits < 10) break;

        std::cout << "Invalid input. Please enter a single digit number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (true) {
        std::cout << "Course Grade (e.g., A+, B, C-): ";
        std::cin >> tempString;
        for (auto& c : tempString) { c = toupper(c); }

        if (POINTS_MAP.contains(tempString)) {
            course.points = POINTS_MAP.at(tempString);
            break;
        }
        std::cout << "Invalid grade entered. Please use a valid grade.\n";
    }

    return course;
}

void addCourse(std::vector<Course>& courses, int& unnamedCounter) {
    courses.push_back(getCourseDetailsFromUser(unnamedCounter));
    std::cout << "Course added successfully!\n";
}

void viewCourses(const std::vector<Course>& courses) {
    if (courses.empty()) {
        std::cout << "No courses to display.\n";
        return;
    }
    int i = 0;
    for (const auto& course : courses) {
        std::cout << "----------------------------------------\n";
        std::cout << ++i << ".\n";
        std::cout << "   Course Name:   " << course.name << "\n";
        std::cout << "   Course Credits: " << course.credits << "\n";
        std::cout << "   Course Grade:   " << GRADE_MAP.at(course.points) << "\n";
    }
    std::cout << "----------------------------------------\n";
}

void editCourse(std::vector<Course>& courses, int& unnamedCounter) {
    viewCourses(courses);
    int choice = 0;
    while (true) {
        std::cout << "Enter the number of the course to edit: ";
        std::cin >> choice;
        if (!std::cin.fail() && choice > 0 && choice <= courses.size()) {
            break;
        }
        std::cout << "Invalid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    courses[choice - 1] = getCourseDetailsFromUser(unnamedCounter);
    std::cout << "Course updated successfully!\n";
}

double calculateGPA(const std::vector<Course>& courses) {
    if (courses.empty()) {
        return 0.0;
    }

    double totalPoints = std::accumulate(courses.begin(), courses.end(), 0.0,
        [] (const double sum, const Course& course) {
            return course.points * course.credits + sum;
        });;
    int totalCredits = std::accumulate(courses.begin(), courses.end(), 0,
        [] (const int sum, const Course& course) {
            return sum + course.credits;
        });

    return totalCredits == 0 ? 0.0 : totalPoints / totalCredits;
}

int totalHours(const std::vector<Course>& courses) {
    return std::accumulate(courses.begin(), courses.end(), 0,
        [](const int sum, const Course& course) {
            return sum + course.credits;
        });
}
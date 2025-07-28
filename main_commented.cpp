/**
 * GPA Calculator - FCAI-CU Students
 * 
 * This program calculates Grade Point Average (GPA) based on the Faculty of Computer 
 * and Artificial Intelligence - Cairo University (FCAI-CU) students bylaw.
 * 
 * Features:
 * - Add, view, edit, and delete courses
 * - Automatic GPA calculation
 * - Data persistence between sessions
 * - Input validation and error handling
 * 
 * Author: [Your Name]
 * Date: [Creation Date]
 * Version: 1.0
 */

#include <iostream>      // For console input/output
#include <string>        // For string manipulation
#include <vector>        // For dynamic course storage
#include <unordered_map> // For efficient grade point lookups
#include <numeric>       // For std::accumulate
#include <limits>        // For input validation
#include <cctype>        // For character case conversion
#include <fstream>       // For file I/O operations

/**
 * Grade Point Mapping Constants
 * 
 * These maps define the conversion between letter grades and grade points
 * according to the FCAI-CU students bylaw.
 */

// Maps letter grades to their corresponding grade points
const std::unordered_map<std::string, double> POINTS_MAP = {
    {"A+", 4.0}, {"A", 3.7}, {"A-", 3.4}, {"B+", 3.2}, {"B", 3.0}, {"B-", 2.8},
    {"C+", 2.6}, {"C", 2.4}, {"C-", 2.2}, {"D+", 2.0}, {"D", 1.5}, {"D-", 1.0}, {"F", 0.0}
};

// Reverse mapping from grade points to letter grades (for display purposes)
const std::unordered_map<double, std::string> GRADE_MAP = {
    {4.0, "A+"}, {3.7, "A"}, {3.4, "A-"}, {3.2, "B+"}, {3.0, "B"}, {2.8, "B-"},
    {2.6, "C+"}, {2.4, "C"}, {2.2, "C-"}, {2.0, "D+"}, {1.5, "D"}, {1.0, "D-"}, {0.0, "F"}
};

/**
 * Course Structure
 * 
 * Represents a single academic course with its associated information.
 */
struct Course {
    std::string name;    // Course name (can be "Unnamed Course X" if left empty)
    int credits;         // Credit hours (0-9, validated range)
    double points;       // Grade points (0.0-4.0, calculated from letter grade)
};

// Forward function declarations
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

/**
 * Main Function
 * 
 * Entry point of the application. Handles the main program loop and
 * coordinates all user interactions.
 */
int main() {
    // Initialize data structures
    std::vector<Course> courses;           // Dynamic storage for all courses
    int unnamedCoursesCounter = 0;         // Counter for auto-generated course names
    int option;                            // User menu choice
    const std::string FILENAME = "gpa_data.txt";  // Data persistence file

    // Load existing course data from file (if any)
    loadCourses(courses, unnamedCoursesCounter, FILENAME);

    // Main program loop
    while (true) {
        // Display the main menu with current GPA and total hours
        printMenu(courses);
        
        // Get user input for menu choice
        std::cout << "Enter your choice: ";
        std::cin >> option;

        // Input validation: check for invalid input (non-numeric)
        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();  // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
            continue;  // Restart loop
        }

        // Process user choice using switch statement
        switch (option) {
            case 1:  // Add a new course
                addCourse(courses, unnamedCoursesCounter);
                break;
                
            case 2:  // View all courses
                viewCourses(courses);
                break;
                
            case 3:  // Edit an existing course
                if (!courses.empty()) {
                    editCourse(courses, unnamedCoursesCounter);
                } else {
                    std::cout << "No courses to edit.\n";
                }
                break;
                
            case 4:  // Delete a course
                if (!courses.empty()) {
                    deleteCourse(courses);
                } else {
                    std::cout << "No courses to delete.\n";
                }
                break;
                
            case 5:  // Exit program
                // Handle exit with save option
                while (true) {
                    std::cout << "Do you want to save your progress? (Yes | No)\n";
                    std::string response;
                    std::cin >> response;
                    
                    // Convert to lowercase for case-insensitive comparison
                    for (auto& c : response) { 
                        c = tolower(c); 
                    }
                    
                    if (response == "yes" || response == "no") {
                        if (response == "yes") {
                            saveCourses(courses, FILENAME);
                            std::cout << "Exiting. Goodbye!\n";
                            return 0;
                        } else {
                            std::cout << "Exiting. Goodbye!\n";
                            return 0;
                        }
                    }
                    std::cout << "Invalid option! Please try again. \n";
                }
                
            default:  // Invalid menu choice
                std::cout << "Invalid option! Please try again.\n";
                break;
        }
        std::cout << "\n";  // Add spacing between menu iterations
    }
}

/**
 * Deletes a course from the collection
 * 
 * @param courses Reference to the vector containing all courses
 */
void deleteCourse(std::vector<Course>& courses) {
    // First, display all courses so user can see what's available
    viewCourses(courses);
    int choice = 0;

    // Input validation loop for course selection
    while (true) {
        std::cout << "Enter the number of the course to edit: ";
        std::cin >> choice;
        
        // Validate input: must be numeric and within valid range
        if (!std::cin.fail() && choice > 0 && choice <= courses.size()) {
            break;
        }
        std::cout << "Invalid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Remove the selected course (convert to 0-based index)
    courses.erase(courses.begin() + --choice);
    std::cout << "Course updated deleted!\n";
}

/**
 * Saves course data to a text file for persistence
 * 
 * @param courses Reference to the vector containing all courses
 * @param filename Name of the file to save data to
 */
void saveCourses(const std::vector<Course>& courses, const std::string& filename) {
    // Open output file stream
    std::ofstream outFile(filename);
    
    // Check if file opened successfully
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for saving: " << filename << std::endl;
        return;
    }

    // Write each course to file (3 lines per course: name, credits, points)
    for (const auto& course : courses) {
        outFile << course.name << '\n';
        outFile << course.credits << '\n';
        outFile << course.points << '\n';
    }
    
    outFile.close();
    std::cout << "Progress saved successfully.\n";
}

/**
 * Loads course data from a text file
 * 
 * @param courses Reference to the vector to populate with courses
 * @param unnamedCounter Reference to counter for unnamed courses
 * @param filename Name of the file to load data from
 */
void loadCourses(std::vector<Course>& courses, int& unnamedCounter, const std::string& filename) {
    // Open input file stream
    std::ifstream inFile(filename);
    
    // If file doesn't exist, return silently (first run)
    if (!inFile.is_open()) {
        return;
    }

    std::string line;
    
    // Read courses in groups of three lines
    while (std::getline(inFile, line)) {
        Course course;
        course.name = line;

        // Read credits (second line)
        if (std::getline(inFile, line)) {
            try { 
                course.credits = std::stoi(line); 
            }
            catch (const std::exception& e) { 
                continue;  // Skip invalid data
            }
        } else break;

        // Read points (third line)
        if (std::getline(inFile, line)) {
            try { 
                course.points = std::stod(line); 
            }
            catch (const std::exception& e) { 
                continue;  // Skip invalid data
            }
        } else break;

        // Update unnamed course counter if this is an unnamed course
        if (course.name.rfind("Unnamed Course ", 0) == 0) {
            try {
                int num = std::stoi(course.name.substr(15));  // Extract number from "Unnamed Course X"
                if (num > unnamedCounter) {
                    unnamedCounter = num;
                }
            } catch (const std::exception& e) {}
        }
        
        // Add valid course to vector
        courses.push_back(course);
    }
    inFile.close();
}

/**
 * Displays the main menu with current GPA and total hours
 * 
 * @param courses Reference to the vector containing all courses
 */
void printMenu(const std::vector<Course>& courses) {
    // Display program header
    std::cout << "==============================================" << std::endl;
    std::cout << "================GPA Calculator================" << std::endl;
    std::cout << "==============================================" << std::endl;

    // Display current statistics if courses exist
    if (!courses.empty()) {
        std::cout.precision(3);  // Set precision for GPA display
        std::cout << "Current GPA: " << calculateGPA(courses) << "\n";
        std::cout << "Total completed hours: " << totalHours(courses) << "\n";
        std::cout << "----------------------------------------------\n";
    }

    // Display menu options
    std::cout << "1. Add a Course" << std::endl;
    std::cout << "2. View Courses" << std::endl;
    std::cout << "3. Edit Courses" << std::endl;
    std::cout << "4. Delete a Course" << std::endl;
    std::cout << "5. Exit" << std::endl;
}

/**
 * Collects course information from user input with validation
 * 
 * @param unnamedCounter Reference to counter for unnamed courses
 * @return Course object with user-provided information
 */
Course getCourseDetailsFromUser(int& unnamedCounter) {
    Course course;
    std::string tempString;

    // Get course name (optional)
    std::cout << "Course Name (Optional): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Clear input buffer
    std::getline(std::cin, course.name);
    
    // Handle empty or "0" input for course name
    if (course.name == "0" || course.name.empty()) {
        course.name = "Unnamed Course " + std::to_string(++unnamedCounter);
    }

    // Get and validate credit hours
    while (true) {
        std::cout << "Course Credits (0-9): ";
        std::cin >> course.credits;
        
        // Validate: must be numeric and in range 0-9
        if (!std::cin.fail() && course.credits >= 0 && course.credits < 10) break;

        std::cout << "Invalid input. Please enter a single digit number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Get and validate letter grade
    while (true) {
        std::cout << "Course Grade (e.g., A+, B, C-): ";
        std::cin >> tempString;
        
        // Convert to uppercase for case-insensitive comparison
        for (auto& c : tempString) { 
            c = toupper(c); 
        }

        // Check if grade exists in our mapping
        if (POINTS_MAP.contains(tempString)) {
            course.points = POINTS_MAP.at(tempString);
            break;
        }
        std::cout << "Invalid grade entered. Please use a valid grade.\n";
    }

    return course;
}

/**
 * Adds a new course to the collection
 * 
 * @param courses Reference to the vector containing all courses
 * @param unnamedCounter Reference to counter for unnamed courses
 */
void addCourse(std::vector<Course>& courses, int& unnamedCounter) {
    // Get course details from user and add to vector
    courses.push_back(getCourseDetailsFromUser(unnamedCounter));
    std::cout << "Course added successfully!\n";
}

/**
 * Displays all courses in a formatted list
 * 
 * @param courses Reference to the vector containing all courses
 */
void viewCourses(const std::vector<Course>& courses) {
    // Handle empty course list
    if (courses.empty()) {
        std::cout << "No courses to display.\n";
        return;
    }
    
    int i = 0;
    // Display each course with numbered formatting
    for (const auto& course : courses) {
        std::cout << "----------------------------------------\n";
        std::cout << ++i << ".\n";
        std::cout << "   Course Name:   " << course.name << "\n";
        std::cout << "   Course Credits: " << course.credits << "\n";
        std::cout << "   Course Grade:   " << GRADE_MAP.at(course.points) << "\n";
    }
    std::cout << "----------------------------------------\n";
}

/**
 * Edits an existing course in the collection
 * 
 * @param courses Reference to the vector containing all courses
 * @param unnamedCounter Reference to counter for unnamed courses
 */
void editCourse(std::vector<Course>& courses, int& unnamedCounter) {
    // First, display all courses so user can see what's available
    viewCourses(courses);
    int choice = 0;
    
    // Input validation loop for course selection
    while (true) {
        std::cout << "Enter the number of the course to edit: ";
        std::cin >> choice;
        
        // Validate input: must be numeric and within valid range
        if (!std::cin.fail() && choice > 0 && choice <= courses.size()) {
            break;
        }
        std::cout << "Invalid number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Replace the selected course with new data (convert to 0-based index)
    courses[choice - 1] = getCourseDetailsFromUser(unnamedCounter);
    std::cout << "Course updated successfully!\n";
}

/**
 * Calculates the weighted Grade Point Average
 * 
 * Formula: GPA = Σ(Grade Points × Credit Hours) / Σ(Credit Hours)
 * 
 * @param courses Reference to the vector containing all courses
 * @return Calculated GPA as a double
 */
double calculateGPA(const std::vector<Course>& courses) {
    // Handle empty course list
    if (courses.empty()) {
        return 0.0;
    }

    // Calculate weighted sum of (points × credits) using std::accumulate
    double totalPoints = std::accumulate(courses.begin(), courses.end(), 0.0,
        [] (const double sum, const Course& course) {
            return course.points * course.credits + sum;
        });
    
    // Calculate total credit hours using std::accumulate
    int totalCredits = std::accumulate(courses.begin(), courses.end(), 0,
        [] (const int sum, const Course& course) {
            return sum + course.credits;
        });

    // Return weighted average, or 0.0 if no credits
    return totalCredits == 0 ? 0.0 : totalPoints / totalCredits;
}

/**
 * Calculates the total credit hours completed
 * 
 * @param courses Reference to the vector containing all courses
 * @return Total credit hours as an integer
 */
int totalHours(const std::vector<Course>& courses) {
    // Sum all course credits using std::accumulate
    return std::accumulate(courses.begin(), courses.end(), 0,
        [](const int sum, const Course& course) {
            return sum + course.credits;
        });
} 
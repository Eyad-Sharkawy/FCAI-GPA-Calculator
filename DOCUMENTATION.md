# GPA Calculator - Technical Documentation

## Table of Contents
1. [Overview](#overview)
2. [Data Structures](#data-structures)
3. [Global Constants](#global-constants)
4. [Function Documentation](#function-documentation)
5. [Main Program Flow](#main-program-flow)
6. [File I/O Operations](#file-io-operations)
7. [Input Validation](#input-validation)
8. [Error Handling](#error-handling)
9. [Memory Management](#memory-management)
10. [Algorithm Details](#algorithm-details)

## Overview

The GPA Calculator is a C++20 console application that manages academic courses and calculates Grade Point Averages. The program follows a modular design with clear separation of concerns between data management, user interface, and calculations.

## Data Structures

### Course Structure
```cpp
struct Course {
    std::string name;    // Course name (can be "Unnamed Course X")
    int credits;         // Credit hours (0-9)
    double points;       // Grade points (0.0-4.0)
};
```

**Purpose**: Represents a single academic course with its associated grade and credit information.

**Fields**:
- `name`: String identifier for the course. If left empty, automatically assigned as "Unnamed Course X"
- `credits`: Integer representing credit hours (validated to be 0-9)
- `points`: Double representing grade points based on letter grade conversion

## Global Constants

### POINTS_MAP
```cpp
const std::unordered_map<std::string, double> POINTS_MAP = {
    {"A+", 4.0}, {"A", 3.7}, {"A-", 3.4}, {"B+", 3.2}, {"B", 3.0}, {"B-", 2.8},
    {"C+", 2.6}, {"C", 2.4}, {"C-", 2.2}, {"D+", 2.0}, {"D", 1.5}, {"D-", 1.0}, {"F", 0.0}
};
```

**Purpose**: Maps letter grades to their corresponding grade points according to FCAI-CU bylaw.

### GRADE_MAP
```cpp
const std::unordered_map<double, std::string> GRADE_MAP = {
    {4.0, "A+"}, {3.7, "A"}, {3.4, "A-"}, {3.2, "B+"}, {3.0, "B"}, {2.8, "B-"},
    {2.6, "C+"}, {2.4, "C"}, {2.2, "C-"}, {2.0, "D+"}, {1.5, "D"}, {1.0, "D-"}, {0.0, "F"}
};
```

**Purpose**: Reverse mapping from grade points to letter grades for display purposes.

## Function Documentation

### Core Functions

#### `int main()`
**Purpose**: Entry point of the application
**Parameters**: None
**Returns**: `int` (exit code)
**Description**: 
- Initializes the course vector and unnamed counter
- Loads existing data from file
- Runs the main program loop with menu-driven interface
- Handles program exit with save option

#### `void printMenu(const std::vector<Course>& courses)`
**Purpose**: Displays the main menu with current GPA and total hours
**Parameters**: 
- `courses`: Reference to vector of courses
**Returns**: `void`
**Description**:
- Shows formatted menu header
- Displays current GPA (3 decimal precision) and total hours if courses exist
- Lists all available menu options

#### `Course getCourseDetailsFromUser(int& unnamedCounter)`
**Purpose**: Collects course information from user input
**Parameters**: 
- `unnamedCounter`: Reference to counter for unnamed courses
**Returns**: `Course` object
**Description**:
- Prompts for course name (optional)
- Validates credit hours (0-9 range)
- Validates letter grade input
- Auto-generates unnamed course names if needed

#### `void addCourse(std::vector<Course>& courses, int& unnamedCounter)`
**Purpose**: Adds a new course to the collection
**Parameters**: 
- `courses`: Reference to vector of courses
- `unnamedCounter`: Reference to unnamed course counter
**Returns**: `void`
**Description**:
- Calls `getCourseDetailsFromUser()` to collect course data
- Appends course to vector
- Provides success feedback

#### `void viewCourses(const std::vector<Course>& courses)`
**Purpose**: Displays all courses in a formatted list
**Parameters**: 
- `courses`: Reference to vector of courses
**Returns**: `void`
**Description**:
- Shows numbered list of all courses
- Displays course name, credits, and letter grade
- Handles empty course list gracefully

#### `void editCourse(std::vector<Course>& courses, int& unnamedCounter)`
**Purpose**: Modifies an existing course
**Parameters**: 
- `courses`: Reference to vector of courses
- `unnamedCounter`: Reference to unnamed course counter
**Returns**: `void`
**Description**:
- Shows course list for selection
- Validates course number input
- Replaces selected course with new data
- Provides success feedback

#### `void deleteCourse(std::vector<Course>& courses)`
**Purpose**: Removes a course from the collection
**Parameters**: 
- `courses`: Reference to vector of courses
**Returns**: `void`
**Description**:
- Shows course list for selection
- Validates course number input
- Removes selected course using vector.erase()
- Provides feedback

### Calculation Functions

#### `double calculateGPA(const std::vector<Course>& courses)`
**Purpose**: Calculates the weighted GPA
**Parameters**: 
- `courses`: Reference to vector of courses
**Returns**: `double` (GPA value)
**Description**:
- Uses `std::accumulate` to calculate weighted sum of (points × credits)
- Calculates total credits
- Returns weighted average: totalPoints / totalCredits
- Returns 0.0 for empty course list

#### `int totalHours(const std::vector<Course>& courses)`
**Purpose**: Calculates total credit hours
**Parameters**: 
- `courses`: Reference to vector of courses
**Returns**: `int` (total hours)
**Description**:
- Uses `std::accumulate` to sum all course credits
- Returns 0 for empty course list

### File I/O Functions

#### `void saveCourses(const std::vector<Course>& courses, const std::string& filename)`
**Purpose**: Saves course data to file
**Parameters**: 
- `courses`: Reference to vector of courses
- `filename`: Output file path
**Returns**: `void`
**Description**:
- Opens output file stream
- Writes each course on three lines: name, credits, points
- Handles file opening errors
- Provides success feedback

#### `void loadCourses(std::vector<Course>& courses, int& unnamedCounter, const std::string& filename)`
**Purpose**: Loads course data from file
**Parameters**: 
- `courses`: Reference to vector of courses
- `unnamedCounter`: Reference to unnamed course counter
- `filename`: Input file path
**Returns**: `void`
**Description**:
- Opens input file stream
- Reads courses in groups of three lines
- Updates unnamed counter for auto-numbering
- Handles file reading errors gracefully
- Uses try-catch for data conversion validation

## Main Program Flow

1. **Initialization**
   - Declare course vector and unnamed counter
   - Load existing data from `gpa_data.txt`

2. **Main Loop**
   - Display menu with current GPA and hours
   - Get user choice (1-5)
   - Execute corresponding function based on choice
   - Continue until exit option selected

3. **Exit Process**
   - Prompt for save confirmation
   - Save data if requested
   - Terminate program

## File I/O Operations

### Data File Format (`gpa_data.txt`)
```
Course Name 1
Credits 1
Points 1
Course Name 2
Credits 2
Points 2
...
```

### Error Handling
- File opening failures are handled gracefully
- Invalid data in file is skipped (try-catch blocks)
- Program continues operation even if file operations fail

## Input Validation

### Credit Hours Validation
- Must be integer
- Range: 0-9
- Clear error messages for invalid input
- Input buffer clearing on failure

### Grade Validation
- Case-insensitive letter grade input
- Must match keys in POINTS_MAP
- Clear error messages for invalid grades
- Automatic uppercase conversion

### Menu Choice Validation
- Must be integer
- Range: 1-5
- Input buffer clearing on failure
- Graceful handling of non-numeric input

### Course Selection Validation
- Must be integer
- Range: 1 to number of courses
- Clear error messages
- Input buffer clearing on failure

## Error Handling

### Input Stream Errors
```cpp
if (std::cin.fail()) {
    std::cout << "Invalid input. Please enter a number.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    continue;
}
```

### File Operation Errors
```cpp
if (!outFile.is_open()) {
    std::cerr << "Error: Could not open file for saving: " << filename << std::endl;
    return;
}
```

### Data Conversion Errors
```cpp
try { 
    course.credits = std::stoi(line); 
}
catch (const std::exception& e) { 
    continue;
}
```

## Memory Management

### Container Usage
- `std::vector<Course>`: Dynamic course storage
- `std::unordered_map`: Efficient grade point lookups
- Automatic memory management through RAII

### Memory Safety
- No manual memory allocation/deallocation
- RAII ensures proper cleanup
- Vector automatically manages course storage
- No memory leaks possible with current implementation

## Algorithm Details

### GPA Calculation Algorithm
```cpp
double calculateGPA(const std::vector<Course>& courses) {
    if (courses.empty()) return 0.0;
    
    double totalPoints = std::accumulate(courses.begin(), courses.end(), 0.0,
        [](const double sum, const Course& course) {
            return course.points * course.credits + sum;
        });
    
    int totalCredits = std::accumulate(courses.begin(), courses.end(), 0,
        [](const int sum, const Course& course) {
            return sum + course.credits;
        });
    
    return totalCredits == 0 ? 0.0 : totalPoints / totalCredits;
}
```

**Formula**: GPA = Σ(Grade Points × Credit Hours) / Σ(Credit Hours)

### Unnamed Course Numbering
- Tracks highest unnamed course number
- Auto-increments for new unnamed courses
- Parses existing unnamed courses on load
- Maintains unique numbering across sessions

## Performance Considerations

### Time Complexity
- GPA calculation: O(n) where n = number of courses
- Course operations (add/edit/delete): O(1) amortized
- File I/O: O(n) for save/load operations

### Space Complexity
- Course storage: O(n) where n = number of courses
- Grade maps: O(1) constant space
- File storage: O(n) proportional to course count

## Future Enhancements

### Potential Improvements
1. **Database Integration**: Replace file I/O with SQLite
2. **GUI Interface**: Add graphical user interface
3. **Semester Management**: Group courses by semester
4. **Grade History**: Track grade changes over time
5. **Export Features**: PDF/Excel report generation
6. **Multiple Grading Scales**: Support different university systems
7. **Course Categories**: Organize by major/minor requirements
8. **Academic Standing**: Calculate academic probation status

### Code Refactoring Opportunities
1. **Class-based Design**: Convert to object-oriented approach
2. **Exception Handling**: Implement custom exception classes
3. **Configuration Files**: Externalize grade point mappings
4. **Unit Testing**: Add comprehensive test suite
5. **Logging System**: Add debug and error logging 
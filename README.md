# GPA Calculator

A simple command-line GPA calculator written in C++ that helps you calculate your Grade Point Average without manual calculations. This calculator is specifically designed based on the **Faculty of Computer and Artificial Intelligence - Cairo University (FCAI-CU) students bylaw**.

## Features

- **Add Courses**: Input course names, credits, and grades
- **View Courses**: Display all entered courses with their details
- **Edit Courses**: Modify existing course information
- **Delete Courses**: Remove courses from your record
- **Automatic GPA Calculation**: Real-time GPA updates as you add/modify courses
- **Data Persistence**: Save and load your course data between sessions
- **Grade Validation**: Supports standard letter grades (A+, A, A-, B+, B, B-, C+, C, C-, D+, D, D-, F)

## Grade Point Scale

The calculator uses the following grade point scale based on the **FCAI-CU students bylaw**:
- A+ = 4.0
- A = 3.7
- A- = 3.4
- B+ = 3.2
- B = 3.0
- B- = 2.8
- C+ = 2.6
- C = 2.4
- C- = 2.2
- D+ = 2.0
- D = 1.5
- D- = 1.0
- F = 0.0

## Building the Project

### Prerequisites
- CMake (version 3.30 or higher)
- C++ compiler with C++20 support

### Build Instructions

1. Clone or download the project files
2. Navigate to the project directory
3. Create a build directory and build the project:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Using an IDE (CLion, Visual Studio, etc.)
- Open the project in your IDE
- The CMakeLists.txt file will automatically configure the build
- Build and run the project from your IDE

## Usage

1. **Run the executable**: `./GPA_Calc` (or `GPA_Calc.exe` on Windows)

2. **Main Menu Options**:
   - **Option 1**: Add a new course
   - **Option 2**: View all courses
   - **Option 3**: Edit an existing course
   - **Option 4**: Delete a course
   - **Option 5**: Exit the program

3. **Adding a Course**:
   - Enter course name (optional - can be left blank for "Unnamed Course")
   - Enter course credits (0-9)
   - Enter letter grade (e.g., A+, B, C-)

4. **Data Persistence**:
   - Your course data is automatically saved to `gpa_data.txt`
   - Data is loaded when you restart the program
   - You can choose to save or discard changes when exiting

## Example Session

```
==============================================
================GPA Calculator================
==============================================
Current GPA: 3.45
Total completed hours: 15
----------------------------------------------
1. Add a Course
2. View Courses
3. Edit Courses
4. Delete a Course
5. Exit
Enter your choice: 1

Course Name (Optional): Introduction to Computer Science
Course Credits (0-9): 3
Course Grade (e.g., A+, B, C-): A
Course added successfully!
```

## File Structure

```
GPA-Calc/
├── main.cpp          # Main source code
├── CMakeLists.txt    # CMake build configuration
├── README.md         # This file
└── gpa_data.txt      # Course data file (created automatically)
```

## Technical Details

- **Language**: C++20
- **Build System**: CMake
- **Data Storage**: Simple text file format
- **Input Validation**: Comprehensive error checking for user inputs
- **Memory Management**: Uses standard C++ containers (std::vector, std::unordered_map)

## Contributing

Feel free to submit issues, feature requests, or pull requests to improve this GPA calculator.

## License

This project is open source and available under the MIT License. 
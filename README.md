# Optimizing-Agriculture



## Overview

This project focuses on wheat crop tracking using digital image analysis. The system utilizes the stb_image library in C and employs sophisticated algorithms for image processing, including resizing, cropping, and pixel analysis. The goal is to provide a robust solution for real-time monitoring and analysis of wheat crop conditions.


## Running the Project

### Environment Setup

Ensure that you have the necessary dependencies installed. These typically include the C compiler and the stb_image library.

### Compilation 

Use the appropriate compiler command to compile the project.
For example: $ clang -o a main.c -lm
Replace main.c with the name of your C source file and a with the desired name for the compiled executable.
The -lm flag is used to link the math library, which might be necessary depending on the math functions used in your code. 
### Execution
Run the compiled executable:
$ ./a

## Features
Image loading and processing with stb_image library.
Resizing and cropping algorithms for efficient data analysis.
Pixel analysis for detecting green and ripe wheat colors.
Real-time monitoring and decision-making for wheat crop conditions.

## Documentation and Code Explanation
For detailed documentation and code explanation, refer to [My Final Report](https://docs.google.com/document/d/1T5V3xuKrQYC54dFMUJxomQEmFo7yxKxH/edit#heading=h.1fob9te). It provides guidelines on running the project, insights into the code structure, and explanations of key algorithms and thresholds.

## Conclusion
This project offers a comprehensive solution for wheat crop tracking, leveraging image analysis techniques. The README provides a brief overview of the project, installation instructions, and details on usage, features, testing, and documentation.


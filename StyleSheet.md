## Project Style Guide

This style guide has been created for this project. The style guide will demonstrate good coding practices and conventions.

## Table of Contents

* Basic Rules
* Class Rules
* Variable Name Rules
* Spacing
* Ordering
* Functions


## Basic Rules

The generic rules for this project are the following:

* Make sure that at the end of every line of text output to the terminal has a newilne character via '\n' or 'endl'. This keeps the output from the code easy to read and understand.
* Spaces between a commented line and the line above it improve readability, so make sure to add an extra line unless it is a block comment that affects multiple lines
* When declaring a function, class, or struct the opening curly bracket must be on the same line as the function signature, class declaration, etc. This makes the code more appealing to look at, and saves space and lines.


## Class Rules

When declaring a class, please use the following guidelines:

* Class names must be descriptive of the class so that it is easier to understand.
* Variables, other objects, and structs must be either protected or private. This ensures that only the base class, or derived class
  can access those variables.
* Variable and function names inside of the class should be descriptive of the function they are performing. This ensures readability
  and understanding throughout the course of the project.

## Variable Name Rules

Please follow the guidelines listed below for variable names:

* All variables should use cammel case, this is a personal choice and should stay consistent throughout the project.
* Variable names should be descriptive, meaning that the variable for the first name of a user should be: fName, firstName, etc.
* Chosing to use the same variable name as a class or struct in a funciton is okay to do. Up to user preference.
    * Ex:
    ```
    class test {
        private:
            string name;
        public:
            setName(string name) { this->name = name; }
    }
    ```
    OR
    ```
    class test {
        private:
            string name;
        public:
            setName(string newName) { name = newName; }
    }
    ```

## Spacing

Spacing is an important factor in readability of your code. Please use the following spacing guidelines:

* When declaring a function, struct, or class make sure to put a space in between the opening curly bracket and whatever is before it.
  This allows the programmer and any other readers to better understand function declarations.
* On line 51 of this file, you can see the following code: '{ this->name = name; }' spaces in between the code and the brackets makes it
  more readable.
* It's okay to not put spaces at the beginning and end of function parameters.

## Ordering

Ordering of functions and variables is important because it can make the code more readable. Use the following guidelines for ordering:

* In classes, variables are defined in the private or protected section. When creating functions to use these variables, order them in the
  same way in which they are declared.
* Constructor functions will always be the first function in a class, any other function(s) after that, and then finally a destructor function.
  This is done to show the life of a class object, from its creation to its destruction.
* Print functions created will be at the top of the function section because they might be used in other functions.

## Functions

Functions are a crucial part of a program and the function prototypes and declarations should be organized. Use the following guidelines:

* Parameters for a function, such as a constructor function should be ordered in the way they are ordered in the class or structure.
* Functions should be short if possible, but if they are long they must be readable.
* Make comments in the function to make sure that everyone can understand what is happening.

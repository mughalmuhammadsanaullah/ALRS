# Automated Logic Reasoning System (ALRS)

A comprehensive C++ application for propositional logic operations,
designed to simplify truth table generation, argument validation, and
logical equivalence checking.

 Table of Contents

-   Overview
-   Features
-   Operators Guide
-   Technical Details
-   Limitations

------------------------------------------------------------------------

 Overview

ALRS is a command-line tool built for discrete mathematics students and
anyone working with propositional logic. It automates tedious manual
calculations and provides instant validation of logical arguments,
making it suitable for study, homework verification, and understanding
logical relationships.

 Features

1. Truth Table Generator
Generate complete truth tables for any propositional logic expression with up to 5 variables.
What it does:

  Displays all possible truth value combinations
  Evaluates the expression for each combination
  Shows final results in an organized table format
  Identifies tautologies and contradictions

2. Argument Validator
Validates logical arguments by checking if conclusions necessarily follow from premises.
What it does:

  Accepts multiple premises (up to 5)
  Tests all possible truth value combinations
  Identifies counterexamples where premises are true but conclusion is false
  Declares arguments as VALID or INVALID with clear explanation

3. Equivalence Checker
Compares two logical expressions to determine if they're logically equivalent.
What it does:

  Generates side-by-side truth tables
  Compares results row by row
  Confirms whether expressions always produce identical results
  Useful for verifying logical laws and simplifications

4. Results Management

Save any output to results.txt file
View previously saved results
Export for homework submission or study materials

 Operators

  Symbol   Meaning
  -------- ------------
  &        AND
  |        OR
  !        NOT
  >       IMPLIES
  =        EQUIVALENT

 Technical Details

-   Infix to postfix conversion using Shunting Yard Algorithm
-   Stack-based postfix evaluation
-   Exhaustive truth table enumeration

 Limitations

-   Maximum 5 variables
-   Propositional logic only
-   Text-based output

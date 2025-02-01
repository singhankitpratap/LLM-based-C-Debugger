# LLM-based-C-Debugger

## Overview
This project utilizes OpenAI's GPT-3.5-Turbo or GPT-4 to assist in debugging C++ code efficiently. The LLM helps by analyzing compiler errors and suggesting minimal code changes while maintaining code integrity.

## LLM Used
- **Model**: OpenAI’s GPT-3.5-Turbo or GPT-4

## Code Execution Steps

### Step 1: Compile the Code
- The C++ code is compiled using the `g++` compiler with the flags `-Wall` (enables warnings) and `-Werror` (treats warnings as errors).
- If compilation is successful, the process terminates.
- If errors or warnings occur, compiler output is captured for analysis.

### Step 2: Parse Compiler Errors
- The compiler output is analyzed to extract all error messages, including:
  - `fatal error`
  - `error`
  - `warning`
- Errors are stored in a list, sorted in their original order.

### Step 3: Debugging Strategy - Fix the Last Error First
- The last error in the stack is selected for debugging.
- **Why the last error?** Fixing the last error often resolves multiple upstream issues simultaneously.
- The specific error message is sent to the LLM for analysis.

### Step 4: LLM Debugging
- The LLM analyzes the code and the error and suggests minimal changes to fix the issue.
- The response includes only the corrected lines in the format:
  ```
  <line number>: <corrected code>
  ```

### Step 5: Apply Fixes
- The corrected lines provided by the LLM are applied directly to the in-memory representation of the code, ensuring only necessary modifications are made.

### Step 6: Recompile and Repeat
- The process continues iteratively until the code compiles successfully or reaches the maximum iteration limit.

## How to Run the Code
1. Generate an OpenAI API key from the OpenAI platform.
2. Replace `"YOUR-OPENAI-API-KEY"` in `debugger.ipynb` with your actual API key.
3. Run the function:
   ```python
   iterative_fix_bottom_up("test2.cpp", max_iterations=10)
   ```

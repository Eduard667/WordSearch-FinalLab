# File Input Output
## 1. Running the Project 
<img width="1483" height="108" alt="Screenshot 2026-02-17 162643" src="https://github.com/user-attachments/assets/a5717b98-fe33-4fef-92b4-0b1f0aba4846" />

This output tells us that we have undefined input and output file
## 2. Adding Comamand Line Arguments
### Steps - Project -> File Input Output Properties -> Debugging -> Command Arguments
<img width="993" height="414" alt="image" src="https://github.com/user-attachments/assets/d439ee03-02a2-4a80-88bd-47eb246b77ad" />
### Output after running with the added Arguments 
<img width="1475" height="240" alt="Screenshot 2026-02-17 164813" src="https://github.com/user-attachments/assets/a38d9543-62fc-44c8-af3e-2e73c53858b8" />

## 2. Adding code 

```cpp
bool Copy(char filenamein[], char filenameout[]) {
    ifstream inputFile(filenamein, ios::in);   // Open for reading
    if (!inputFile) { // Check if input file exists
        cerr << "Error: Could not open source file '" << filenamein << "'\n";
        return false;
    }

    ofstream outputFile(filenameout, ios::out); // Open for writing
    if (!outputFile) { // Check if output file can be created
        cerr << "Error: Could not create output file '" << filenameout << "'\n";
        return false;
    }

    char ch;
    while (inputFile.get(ch)) { // Read one character at a time
        outputFile.put(ch);     // Write character to output file
    }

    if (inputFile.bad() || outputFile.bad()) {
        cerr << "Error: File read/write error occurred.\n";
        return false;
    }

    inputFile.close();
    outputFile.close();
    return true;
}
```
### What has been changed: 
Opened input file with ifstream and checked if it exists.
Opened output file with ofstream and checked if it can be created.
Copied character-by-character using get() and put().
Added error checks for read/write failures.
Closed both files after copying.
Returned true/false to indicate success or failure.

### Reflection
1. I changed the arguments, to see what will happen: 
<img width="509" height="72" alt="Screenshot 2026-02-17 180505" src="https://github.com/user-attachments/assets/3c9fad20-f2d2-4361-b942-e65d255240a9" />

2. Output of new arguments
<img width="599" height="148" alt="image" src="https://github.com/user-attachments/assets/7fb49ea3-6fd1-45da-8903-d197f87a8373" />

3. Changed the input file name back to input.txt, and program runs, the output file name can be any name:
<img width="486" height="76" alt="image" src="https://github.com/user-attachments/assets/d032777e-8182-48b5-a5e8-e003091203cb" />
<img width="430" height="129" alt="image" src="https://github.com/user-attachments/assets/6d3e0dda-a449-4ba5-9a41-05545620e504" />




### Output: 
<img width="1475" height="222" alt="image" src="https://github.com/user-attachments/assets/bed9cb97-730e-4a1c-9f02-544aca14b9e3" />

 # CSV_Parser

 ### Description:
 This is the repository for my Comma-Separated Value file parser. The worikings are simple: The program reads values from a text file and forms a table, where each comma separates two columns and each new line is a new row in the table respectively.
 
 It has a simple UI, that takes user input and manipulates files and tables accordingly. It supports the following funcalities:

 1. open - reads and creates a table with said name. If it exists, the data is loaded in the program, but if it does not, an empty table is loaded and appropriate error indication is shown.

 2. close - closes current table. Cannot be called while outside of a table. After closing the user can either open a new table or exit entirely.

 3. save - writes current table as comma-separated values to the filename specified beforehand.

 4. save as - writes current table as comma-separated values to a new filename.

 5. print types - prints a version of the table, with the types of the cell values. There are four possible types: Integer and Decimal are for numbers, Quoted is for text that starts and ends with quotes, and anything that does not fit into any of these three is considered Invalid.

 6. edit - edits the data of a table cell if the provided value or formula is valid.

 7. exit - terminates and exits the program.

 ### Disclaimer!
 The intended use of the UI functions, only requires calling the function by name (Example: "open", not "open (some name)"). If an incorrectly formatted command is entered the user will be prompted to do so again.

 ##### Author: Angel Ivanov
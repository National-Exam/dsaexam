#include <iostream>
#include <string>
#include <iomanip>
#include<fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
using namespace std;

string helpMenu () 
{
    string headerLine = "-----------------------------------------------\n";
    string headerTitle = "*           Command syntaxes           * \n";
    string helpCommands =
        "itemadd <item_id> <item_name> <quantity> <registration_date>                  :Add a new item in the inventory \n"       
        "itemslist                                                                     :Lists items in the inventory \n"
        "help                                                                          :Prints help messages \n"
        "exit                                                                          :Exit the program \n";


        return headerLine + headerTitle + headerLine + helpCommands;
        
}
bool isValidDate(string date) {
  regex pattern("^(\\d{4})-(\\d{1,2})-(\\d{1,2})$");
  smatch matches;

  if (!regex_match(date, matches, pattern)) {
    return false;
  }

  int month = stoi(matches[2]);
  int day = stoi(matches[3]);

  if (month > 12) {
    return false;
  }

  if (day > 31) {
    return false;
  }

  return true;
}
bool isNumber(string string) {
  regex pattern("^[0-9]+$");
  return regex_match(string, pattern);
}
bool validateItemData(vector<string> item_data) {  

  // Validate the first and third items are numbers.
  if (!isNumber(item_data[1])) {
    return false;
  }

  if (!isNumber(item_data[3])) {
    return false;
  }

  // Validate the fourth item is a date in the format year-month-day.  
  if (!isValidDate(item_data[4])) {
    return false;
  }

  return true;
} 
// To create the header file if it is not already in the file.
void createHeaderLine (string filename) {  
  vector<string> header;
  header.push_back("Item Id");
  header.push_back("Item Name");
  header.push_back("Quantity");
  header.push_back("Reg Date");

  // Check if the file exists.
  ifstream infile(filename);
  if (!infile.is_open()) {
    // The file does not exist, so create it and add the header.
    ofstream outfile(filename);
    outfile << header[0] << "," << header[1] << "," << header[2] << "," << header[3] << endl;
    outfile.close();
  } else {
    // The file exists, so check if it has a header.
    string line;
    getline(infile, line);
    if (line != header[0] + "," + header[1] + "," + header[2] + "," + header[3]) {
      // The file does not have a header, so add it.
      ofstream outfile(filename, ios::app);
      outfile << header[0] << "," << header[1] << "," << header[2] << "," << header[3] << endl;
      outfile.close();
    }
  }

  // Close the file.
  infile.close();

}
void addItem( string item_id, string item_name, string item_quantiry,string item_registration_date, string fileName)
{
       string data =item_id + "," + item_name + "," + item_quantiry + "," + item_registration_date;
        createHeaderLine(fileName);
        ofstream outputFile;
        outputFile.open(fileName, ios::app);
        if(!outputFile.is_open()){
            cout << "Error opening file" << endl;
        }else{
            outputFile << data << endl;
        }
    outputFile.close();
}


// This will split the string input
vector<string> splitString(const string &input)
{
    vector<string> tokens;
    istringstream iss(input);
    string token;

    while (iss >> token)
    {
            tokens.push_back(token);
    }

    return tokens;
}


// This function will list items recorded in the csv
void listItems(const string& filename, int sortColumn) {
    // Open the CSV file
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open the file." << endl;
        return;
    }

    // Read the header line
    string headerLine;
    getline(file, headerLine);

    // Split the header line into separate headers
    istringstream headerStream(headerLine);
    vector<string> headers;
    string header;
    while (getline(headerStream, header, ',')) {
        headers.push_back(header);
    }

    // Read and process the data lines
    vector<vector<string>> data;
    string dataLine;
    while (getline(file, dataLine)) {
        istringstream dataStream(dataLine);
        string value;
        vector<string> rowData;
        while (getline(dataStream, value, ',')) {
            rowData.push_back(value);
        }
        data.push_back(rowData);
    }

    // Sort the data based on the specified column
    sort(data.begin(), data.end(), [sortColumn](const vector<string>& a, const vector<string>& b) {
        return a[sortColumn] < b[sortColumn];
    });

    // Display the sorted data with headers        
     int headerIndex = 0;
    for (const auto& rowData : data) {
        for (const auto& value : rowData) {            
            cout  <<  headers[headerIndex] << " :" <<  setw(7) << left << value << " ";
            headerIndex++;
        }
        headerIndex = 0;
        cout << endl;
    }

    // Close the file
    file.close();
}



int main()

{
    string command;   
    cout << "   *********************************************\n\n";
    cout << "*  Welcome to To RCA inventory management system   * \n\n";
    cout << "   **********************************************  \n";    
    cout << "Need a help? Type 'help' then press Enter key.!\n";        
   
    // Convert the user input to lowercase for case-insensitive comparison
    do{
        cout << "Enter a command: ";
         getline(cin, command);
  for (char &c : command)
    {
        c = tolower(c);
    }
   if (command == "help")
    {
        cout << helpMenu() << endl;
        continue;
    }   
    else if (splitString(command)[0] == "itemadd"){
        vector<string> itemData = splitString(command);        
        bool valid = validateItemData(itemData); 
        if(!valid){
           cout << "Please input valid data in this format: item Id: number,item name: text, quantity: number, date: year-month-day \n";
        }
        else{
         addItem(itemData[1],itemData[2],itemData[3],itemData[4],"items.csv");
         cout << "Item recorded successfully" << endl;
        }
        
    }
    else if (splitString(command)[0] == "itemslist"){
       listItems("items.csv",1);               
    }   
    else if(command != "exit")
    {
        cout << "Invalid command, Type help to view available commands." << endl;
    }
    }while(command != "exit");
  

    return 0;
}


// improvements
// I will need to check if an item with id already exists and notify the user.
// Improve the algorithm and run time.
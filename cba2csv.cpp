/*
 * =====================================================================================
 *
 *       Filename:  cba2csv.cpp
 *
 *    Description:  Converts a CBA bank statement to a .csv file
 *
 *        Version:  1.0
 *        Created:  07/07/2011 06:04:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Matthew Sherborne (), msherborne@gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include "pdf2txt.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <fstream>

using namespace std;

/**
* @brief Parses a vector of strings and outputs a csv text to a stream
*/
class Parser {
private:
    vector<string>::const_iterator iter;
    vector<string>::const_iterator end;
    unsigned short start_day;
    unsigned short start_month;
    unsigned short start_year;
    unsigned short end_day;
    unsigned short end_month;
    unsigned short end_year;
    vector<string> data;
    ostream& output;

public:
    /**
    * @brief The main parsing loop
    *
    * @param filename pdf filename to read
    * @param output the stream to output csv to
    */
    Parser(char* filename, ostream& output) : output(output) {
        pdf2txt::pdf2txt(filename, data);
        iter = data.begin();
        end = data.end();
        findDate("Statement begins", start_day, start_month, start_year);
        findDate("Statement ends", end_day, end_month, end_year);
        while (iter != end)
            if (findStatementBlock())
                while (iter != end)
                    if (!outputStatementLine())
                        break;
    }

private:
    /**
    * @brief Turns a month name into an integer. Jan=1
    *
    * @param monthName 3 letter sentence case month name
    *
    * @return integer representing that month
    */
    inline unsigned short monthName2num(const string& monthName) {
        return monthName == "Jan" ? 1 :
               monthName == "Feb" ? 2 :
               monthName == "Mar" ? 3 :
               monthName == "Apr" ? 4 :
               monthName == "May" ? 5 :
               monthName == "Jun" ? 6 :
               monthName == "Jul" ? 7 :
               monthName == "Aug" ? 8 :
               monthName == "Sep" ? 9 :
               monthName == "Oct" ? 10 :
               monthName == "Nov" ? 11 :
               monthName == "Dec" ? 12 :
               0;
    }

    /**
    * @brief Finds and decodes a date.
    * 
    * First it'll find a string that matches the marker.
    * Then it'll read the next string along and decode the date informaton from it
    *
    * @param marker Search for a string that matches this marker
    * @param day The place to store the day of the month
    * @param month The place to store the month number
    * @param year The place to store the year number
    */
    inline void findDate(const string& marker, unsigned short& day, unsigned short& month, unsigned short& year) {
        while (iter != end) {
            string line = *iter++;
            if (line == marker) {
                line = *iter;
                // Read the date line
                stringstream parts(line);
                string monthName;
                parts >> skipws >> day >> monthName >> year;
                month = monthName2num(monthName);
                // Return if we got it
                if ((day != 0) && (month != 0) && (year != 0))
                    return;
            }
        }
        throw invalid_argument("Couldn't parse date or find marker: " + marker);
    }

    /**
    * @brief Finds the start of a block of statements
    *
    * @return true if it found the start of a block. false if it ate the whole input stream.
    */
    inline bool findStatementBlock() {
        string sDate = "Date";
        string sTransactionDetails = "Transaction details";
        string sAmount = "Amount (A$)";
        while (iter != end) {
            if (*iter++ == "Date")
                if(*iter++ == "Transaction details")
                    if(*iter++ == "Amount (A$)")
                        return true;
        }
        return false;
    }

    /**
    * @brief Reads in a line from a statement block and outputs CSV
    *
    * @return Returns false if it finds a line that doesn't parse.
    */
    inline bool outputStatementLine() {
        // Read the date
        stringstream date(*iter++);
        string monthName;
        unsigned short day, month, year;
        date >> skipws >> day >> monthName;
        month = monthName2num(monthName);
        if ((day == 0) || (month == 0))
            return false;
        // Work out the year
        year = ((month <= 12) && (month >= start_month) && (day >= start_day)) ? start_year : end_year;
        // Read the transaction details
        string details = *iter++;
        // Reformat for proper CSV
        size_t index = details.find('"');
        while (index != string::npos) {
            details.replace(index, 1, "\"\"");
            index += 2;
            if (index < details.length())
                index = details.find('"', index);
            else
                break;
        }
        // Read the amount
        int dollars;
        char dot;
        unsigned short cents;
        stringstream amount(*iter++);
        amount >> skipws >> dollars >> dot >> cents;
        // Skip over the transaction ID
        ++iter;
        // Output a line of CSV
        output << day << '/' << month << '/' << year << ','
               << '"' << details << '"' << ','
               << dollars << '.' << cents << endl;
        return true;
    }

};

int main ( int argc, char *argv[] ) {
    // Get the output filename
    string filename = argv[1];
    size_t index = filename.rfind('.');
    string ext = filename.substr(index+1, 3);
    cout << ext << endl;
    if (ext != "csv")
        filename.replace(index+1, 3, "csv");
    else
        filename += ".csv";
    ofstream output(filename.c_str());
    // Output the csv header
    output << "date,comment,amount" << endl;
    // Output the contents
    Parser parser(argv[1], output);
}

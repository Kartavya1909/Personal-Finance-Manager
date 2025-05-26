#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
 
using namespace std;
 
struct Transaction {
    string type;
    string category;
    double amount;
    string date;
};

// Input validation for positive amounts
double getValidAmount() {
    double amount;
    while(true) {
        cout << "Enter Amount: ";
        cin >> amount;
        if(amount > 0) {
            return amount;
        }
        cout << "Please enter a positive amount!" << endl;
    }
}

// Enhanced transaction addition with validation
void addTransaction(vector<Transaction> &transactions, const string &type) {
    Transaction t;
    t.type = type;

    cout << "Enter Category: ";
    cin.ignore();
    getline(cin, t.category);
    
    if(t.category.empty()) {
        t.category = "General";
    }

    t.amount = getValidAmount();

    cout << "Enter date (DD/MM/YYYY): ";
    cin >> t.date;

    transactions.push_back(t);
    cout << "Transaction added successfully!" << endl;
}

// Enhanced view with better formatting
void viewAll(const vector<Transaction> &transactions) {
    if(transactions.empty()) {
        cout << "No transactions found!" << endl;
        return;
    }
    
    cout << "\n==== ALL TRANSACTIONS ====" << endl;
    cout << left << setw(10) << "Type" << setw(15) << "Category" 
         << setw(10) << "Amount" << setw(12) << "Date" << endl;
    cout << string(50, '-') << endl;
    
    for(const Transaction &t : transactions) {
        cout << left << setw(10) << (t.type == "inc" ? "Income" : "Expense")
             << setw(15) << t.category 
             << setw(10) << fixed << setprecision(2) << t.amount 
             << setw(12) << t.date << endl;
    }
}

// Enhanced reporting with category breakdown
void viewReport(const vector<Transaction> &transactions) {
    if(transactions.empty()) {
        cout << "No transactions to report!" << endl;
        return;
    }
    
    double totalIncome = 0, totalExpense = 0;
    map<string, double> incomeByCategory, expenseByCategory;
    
    for(const Transaction &t : transactions) {
        if(t.type == "inc") {
            totalIncome += t.amount;
            incomeByCategory[t.category] += t.amount;
        } else {
            totalExpense += t.amount;
            expenseByCategory[t.category] += t.amount;
        }
    }
    
    cout << fixed << setprecision(2);
    cout << "\n==== FINANCIAL SUMMARY ====" << endl;
    cout << "Total Income:  $" << totalIncome << endl;
    cout << "Total Expense: $" << totalExpense << endl;
    cout << "Net Balance:   $" << (totalIncome - totalExpense) << endl;
    
    if(totalIncome < totalExpense) {
        cout << "⚠️  You're spending more than you earn!" << endl;
    }
    
    // Category breakdown
    cout << "\n--- Income by Category ---" << endl;
    for(const auto &pair : incomeByCategory) {
        cout << pair.first << ": $" << pair.second << endl;
    }
    
    cout << "\n--- Expenses by Category ---" << endl;
    for(const auto &pair : expenseByCategory) {
        cout << pair.first << ": $" << pair.second << endl;
    }
}

// Enhanced file operations with error handling
void saveToFile(const vector<Transaction> &transactions, const string &filename) {
    ofstream file(filename);
    if(!file.is_open()) {
        cout << "Error: Could not save to file!" << endl;
        return;
    }
    
    // Save with header
    file << "Type,Category,Amount,Date" << endl;
    for(const Transaction &t : transactions) {
        file << t.type << "," << t.category << "," << t.amount << "," << t.date << endl;
    }
    file.close();
    cout << "Data saved successfully!" << endl;
}

void loadFromFile(vector<Transaction> &transactions, const string &filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        cout << "No existing data file found. Starting fresh!" << endl;
        return;
    }
    
    string line;
    transactions.clear();
    
    // Skip header if present
    getline(file, line);
    if(line != "Type,Category,Amount,Date") {
        file.seekg(0); // Go back to beginning if no header
    }
    
    while(getline(file, line)) {
        stringstream ss(line);
        string type, category, strAmount, date;
        
        if(getline(ss, type, ',') && getline(ss, category, ',') && 
           getline(ss, strAmount, ',') && getline(ss, date)) {
            
            Transaction t;
            t.type = type;
            t.category = category;
            t.amount = stod(strAmount);
            t.date = date;
            transactions.push_back(t);
        }
    }
    file.close();
    cout << "Loaded " << transactions.size() << " transactions successfully!" << endl;
}

int main() {
    vector<Transaction> transactions;
    string filename = "file.csv";

    cout << "==== Personal Finance Manager ====" << endl;
    loadFromFile(transactions, filename);

    int choice;
    do {
        cout << "\n==== MENU ====" << endl;
        cout << "1. Add Income" << endl;
        cout << "2. Add Expense" << endl;
        cout << "3. View All Transactions" << endl;
        cout << "4. View Financial Summary" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";

        cin >> choice;

        switch(choice) {
            case 1:
                addTransaction(transactions, "inc");
                saveToFile(transactions, filename);
                break;
            case 2:
                addTransaction(transactions, "exp");
                saveToFile(transactions, filename);
                break;
            case 3:
                viewAll(transactions);
                break;
            case 4:
                viewReport(transactions);
                break;
            case 5:
                cout << "Thank you for using Personal Finance Manager!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 5);

    return 0;
}
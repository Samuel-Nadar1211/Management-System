// C++ Program to create a Library Management System

#include <iostream>
#include <string>
#include <vector>

using namespace std;


class DateDifference // Assume user will only enter valid date in ddmmyyyy format
{
    int day, month, year;
    int totalDay() // from 31121999
    {
        int count = day;
        for (int i = 1; i < month; i++)
        {
            if (i == 2)
                if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
                    day += 29; // leap year
                else
                    count += 28;

            else if (i == 4 || i == 6 || i == 9 || i == 11)
                count += 30;
            else
                count += 31;
        }

        count += (year - 2000) * 365;
        count += (year - 1) / 4 - (year - 1) / 100 + (year - 1) / 400 - 484; // leap days
        // 1999/4 - 1999/100 + 1999/400 = 499 - 19 + 4 = 484
        return count;
    }

public:
    DateDifference(int date)
    {
        year = date % 10000;
        month = (date / 10000) % 100;
        day = date / 1000000;
    }

    int operator-(DateDifference issue_date)
    {
        return totalDay() - issue_date.totalDay();
    }
};


class InventoryManagement
{
protected:
    class Book
    {
    public:
        string name;
        int available_copy;
        vector<int> reserved_member;

        Book(string name, int number_of_copy)
        {
            this->name = name;
            available_copy = number_of_copy;
        }
    };
    vector<Book> inventory;

    void addBook(string name, int copy = 1)
    {
        for (int i = 0; i < inventory.size(); i++)
            if (inventory[i].name == name)
            {
                inventory[i].available_copy += copy;
                cout << copy << " copy of book '" << name << "' is added\n";
                return;
            }

        inventory.push_back({name, copy});
        cout << copy << " copy of book '" << name << "' is added\n";
    }

    InventoryManagement() // Default Constructor
    {
        addBook("Wings of Fire", 5);
        addBook("Don Quixote", 2);
        addBook("Robinson Crusoe", 4);
        addBook("The Alchemist", 4);
        addBook("The Tempest", 3);
        addBook("Refugee", 3);
    }
};

class UserManagement
{
protected:
    class LibraryMember
    {
        int fine_paid = 0;

    public:
        string name;
        int id;
        vector<string> book_borrowed;
        vector<DateDifference> issued_date;

        LibraryMember(string name, int id)
        {
            this->name = name;
            this->id = id;
        }

        int calculateFine(int day)
        {
            if (day <= 14)
                return 0;
            else if (day <= 28)
                return (day - 14) * 2;
            else
                return 14 * 2 + (day - 28) * 5;
        }

        int calculateFineOwned(int date)
        {
            int fine = 0;
            for (int i = 0; i < book_borrowed.size(); i++)
                fine += calculateFine(DateDifference(date) - issued_date[i]);
            return fine;
        }

        void payFine(int fine)
        {
            fine_paid += fine;
        }

        void getFinePaid()
        {
            cout << name << "'s total fine paid is " << fine_paid << endl;
        }
    };
    vector<LibraryMember> members;

    void createMember(string name)
    {
        static int id = 1000;
        members.push_back(LibraryMember({name, id}));
        cout << name << " is now a member with id " << id << endl;
        id++;
    }

    UserManagement()
    {
        createMember("Samuel");
        createMember("Simon");
        createMember("Abigail");
        createMember("Nivetha");
    }
};


class LibraryManagement : public UserManagement, public InventoryManagement
{
    void updateReservation(vector<Book>::iterator book, int date)
    {
        for (int i = 0; i < book->reserved_member.size(); i++)
        {
            int j;
            for (j = 0; j < members.size(); j++)
                if (book->reserved_member[i] == members[j].id)
                    break;

            if (members[j].calculateFineOwned(date)) // If fine owned reservation cancelled
            {
                book->reserved_member.erase(book->reserved_member.begin() + i);
                i--;
            }
        }
    }

protected:
    void requestBook(int id, string book_name, int date)
    {
        vector<LibraryMember>::iterator member;
        for (member = members.begin(); member != members.end(); member++)
            if (member->id == id)
                break;
        
        if (member == members.end())
        {
            cout << "Unregistered Member!!\n";
            return;
        }

        vector<Book>::iterator book;
        for (book = inventory.begin(); book != inventory.end(); book++)
            if (book->name == book_name)
                break;

        if (book == inventory.end())
        {
            cout << "Book not found!!\n";
            return;
        }

        updateReservation(book, date);

        if (book->available_copy)
            for (int i = 0; i < book->reserved_member.size(); i++)
                if (book->reserved_member[i] == id)
                {
                    member->issued_date.push_back(DateDifference(date));
                    member->book_borrowed.push_back(book_name);

                    book->available_copy--;
                    book->reserved_member.erase(book->reserved_member.begin() + i);

                    cout << book_name << " is issued to " << member->name << endl;
                    return;
                }

        if (book->available_copy > book->reserved_member.size())
        {
            member->issued_date.push_back(DateDifference(date));
            book->available_copy--;
            member->book_borrowed.push_back(book_name);
            cout << book_name << " is issued to " << member->name << endl;
        }

        else if (!member->calculateFineOwned(date))
        // If no fine owned then reserve facility available
        {
            book->reserved_member.push_back(id);
            cout << book_name << " is reserved for " << member->name << endl;
        }

        else
            cout << "Book not available presently\n";
    }

    void returnBook(int id, string book_name, int date)
    {
        vector<LibraryMember>::iterator member;
        for (member = members.begin(); member != members.end(); member++)
            if (member->id == id)
                break;
        
        if (member == members.end())
        {
            cout << "Unregistered Member!!\n";
            return;
        }

        int i;
        for (i = 0; i < member->book_borrowed.size(); i++)
            if (member->book_borrowed[i] == book_name)
                break;
        
        if (i == member->book_borrowed.size())
        {
            cout << book_name << " not borrowed!!\n";
            return;
        }

        int fine = member->calculateFine(DateDifference(date) - member->issued_date[i]);
        cout << "Fine is Rs." << fine << endl;
        member->payFine(fine);

        member->book_borrowed.erase(member->book_borrowed.begin() + i);
        member->issued_date.erase(member->issued_date.begin() + i);
        
        vector<Book>::iterator book;
        for (book = inventory.begin(); book != inventory.end(); book++)
            if (book->name == book_name)
                break;
                
        book->available_copy++;
        
        cout << book_name << " is successfully returned by " << id << endl;
    }

    void searchBook(string search)
    {
        bool book_found = false;
        cout << "Search result:\n";
        
        for (vector<Book>::iterator book = inventory.begin(); book != inventory.end(); book++)
            if (book->name.size() >= search.size())
            {
                bool match = true;
                for (int i = 0; i < search.size(); i++)
                    if (search[i] != book->name[i])
                    {
                        match = false;
                        break;
                    }
                
                if (match)
                {
                    book_found = true;
                    cout << book->name << endl;
                }
            }
        
        if (!book_found)
            cout << "No such book exists in the Inventory!!\n";
    }
};

class CommandPrompt : protected LibraryManagement
{
public:
    CommandPrompt()
    {
        cout << "\nCommands available are:\nadd_new_book no_of_copy book_name\ncreate_user name\nissue id issue_date book_name\nreturn id return_date book_name\nsearch book_name\nfind_fine_owned id date\nfind_fine_paid id\nstop\n\n";
        cout << "Note- Enter date in ddmmyyyy format\n\n------------------------------------------------------\n\n";
        
        cout << ">>> ";
        string command;
        cin >> command;

        while (command != "stop")
        {
            if (command == "add_new_book")
            {
                int copy;
                cin >> copy;
                
                string name;
                getline(cin, name);
                name.erase(name.begin());
                
                addBook(name, copy);
            }
            
            else if (command == "create_user")
            {
                string name;
                cin >> name;
                createMember(name);
            }
            
            else if (command == "issue")
            {
                int id, date;
                cin >> id >> date;
                
                string name;
                getline(cin, name);
                name.erase(name.begin());

                requestBook(id, name, date);
            }
            
            else if (command == "return")
            {
                int id, date;
                cin >> id >> date;
                
                string name;
                getline(cin, name);
                name.erase(name.begin());
                
                returnBook(id, name, date);
            }

            else if (command == "search")
            {
                string search;
                getline(cin, search);
                search.erase(search.begin());
                
                searchBook(search);
            }

            else if (command == "find_fine_owned")
            {
                int id, date;
                cin >> id >> date;
                
                for (int i = 0; i < members.size(); i++)
                    if (members[i].id == id)
                    {
                        cout << members[i].name << "'s total fine owned is " << members[i].calculateFineOwned(date) << endl;
                        break;
                    }
            }

            else if (command == "find_fine_paid")
            {
                int id;
                cin >> id;
                
                for (int i = 0; i < members.size(); i++)
                    if (members[i].id == id)
                    {
                        members[i].getFinePaid();
                        break;
                    }
            }

            else
            {
                string trash;
                getline(cin, trash);
                cout << "Invalid Command!!\n";
            }

            cout << "\n>>> ";
            cin >> command;
        }
    }
};


int main()
{
    CommandPrompt shell;
    return 0;
}
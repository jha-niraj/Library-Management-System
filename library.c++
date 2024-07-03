// Library Management System using C++:
// Features are like:
/*
    1. Add New Books -> Done
    2. Search for a Book -> Done
    3. Issue a Book -> Done
    4. Return a Book -> Done
    5. List all Books -> Done
    6. Delete a Book -> Done
*/

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<queue>
using namespace std;

class Student {
    public:
        int id;
        string name;
};

class Book {
    public:
        int id;
        string title;
        string author;
        bool status;
        Student* issuedTo;

        Book(int _id, string _title, string _author, bool _status) {
            this -> id = _id;
            this -> title = _title;
            this -> author = _author;
            this -> status = _status;
            this -> issuedTo = nullptr;
        }
};

class Library {
    private:
        vector<Book> books;
        queue<pair<int, Student>> issueQueue;
    
    public:
        void addNewBook(int id, string title, string author) {
            books.push_back(Book(id, title, author, true));
            sort(books.begin(), books.end(), [](const Book& a, Book& b) {
                return a.id < b.id;
            });
        }

        /*
            Searching algorithm:
                We have to write two search algorithm:
                    1. For the string query like title -> We can just traverse the book array and find it if available
                    2. For the id which is integer -> We are going to use the binary search to find the id.
        */
        Book* searchByTitle(const string &title) {
            for(auto& book : books) {
                if(book.title == title) {
                    return &book;
                }
            }
            return nullptr;
        }
        Book* searchById(int id) {
            int s = 0, e = books.size() - 1;
            while(s <= e) {
                int mid = s + (e - s) / 2;
                if(books[mid].id == id) {
                    return &books[mid];
                } else if(mid > id) {
                    e--;
                } else {
                    s++;
                }
            } 
            return nullptr;
        }
        // Lastly we have an function to search either the user passes the integer as id or title as string:
        void searchBook(const string & query) {
            try {
                int id = stoi(query);
                Book* book = searchById(id);
                if(book) {
                    displayBookDetails(book);
                } else {
                    cout << "No book with this id is found!!!";
                }
            } catch(invalid_argument&) {
                Book* book = searchByTitle(query);
                if(book) {
                    displayBookDetails(book);
                } else {
                    cout << "No book with this title is found!!!";
                }
            }
        }

        // Issue a Book and return an Book:
        bool issueBook(int bookId, const Student& student) {
            Book* book = searchById(bookId);
            if(book && book -> status) {
                book -> status = false;
                book -> issuedTo = new Student(student);
                issueQueue.push({ bookId, student });
                cout << "Book '" << book->title << "' issued to " << student.name << endl;
                return true;
            } else if(book) {
                cout << "Book with Id " << bookId << " is already issued" << endl;
            } else {
                cout << "Book with ID " << bookId << " not found." << endl;
            }
            return false;
        }
        bool returnBook(int bookId) {
            Book* book = searchById(bookId);
            if (book && !book -> status) {
                cout << "Book '" << book->title << "' returned by " << book->issuedTo->name << endl;
                book -> status = true;
                delete book -> issuedTo;
                book -> issuedTo = nullptr;
            
                if(!issueQueue.empty() && issueQueue.front().first == bookId) {
                    issueQueue.pop();
                }
                return true;
            } else if (book) {
                std::cout << "Book '" << book->title << "' is not currently issued." << std::endl;
            } else {
                std::cout << "Book with ID " << bookId << " not found." << std::endl;
            }
            return false;
        }
        void displayIssueQueue() {
            cout << "Current Issue Queue:" << endl;
            auto tempQueue = issueQueue;
            while (!tempQueue.empty()) {
                auto issue = tempQueue.front();
                cout << "Book ID: " << issue.first << ", Issued to: " << issue.second.name << endl;
                tempQueue.pop();
            }
        }

        void displayAllBooks() {
            for(const auto& book: books) {
                cout << "Id: " << book.id << endl;
                cout << "Title: " << book.title << endl;
                cout << "Author: " << book.author << endl;
                cout << "Status: " << (book.status ? "Available" : "Issued") << endl;
            }
        }

        void displayBookDetails(const Book* book) {
            cout << "Book found:" << std::endl;
            cout << "ID: " << book->id << std::endl;
            cout << "Title: " << book->title << std::endl;
            cout << "Author: " << book->author << std::endl;
            cout << "Status: " << book -> status << std::endl;
        }

        bool deleteBook(int id) {
        auto it = find_if(books.begin(), books.end(), [id](const Book& book) { return book.id == id; });
        if (it != books.end()) {
            cout << "Deleting book: " << it->title << " by " << it->author << std::endl;
            books.erase(it);
            return true;
        } else {
            cout << "Book with ID " << id << " not found." << std::endl;
            return false;
        }
    }
};

// Function to support the switch case:
void bookAdditionOperation(Library& lib) {
    // This is for the admin only:
    int bookToAdd;
    cout << "How many books do you want to add? ";
    cin >> bookToAdd;

    while(bookToAdd) {
        int id;
        string title;
        string author;
        string status;

        cout << "Enter the id for the book " << ": ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Enter the title for the book " << ": ";
        getline(cin, title);
        
        cout << "Enter the author for the book " << ": ";
        getline(cin, author);

        lib.addNewBook(id, title, author);
        bookToAdd--;
        cout << endl;
    }
}
void bookSearchOperation(Library& lib) {
    // Search the book by the id as well as title:
    string searchQuery;

    cout << "Enter the title or id of the book you want to search for: ";
    cin >> searchQuery;

    lib.searchBook(searchQuery);
    cout << endl;
}
void bookIssueOperation(Library& lib) {
    // Initially, there are 3 student in the Library who want book:
    Student student1 = {101, "Niraj Jha"};
    Student student2 = {102, "Sonali Jha"};
    Student student3 = {103, "Shruti Jha"};

    lib.issueBook(1, student1);
    lib.issueBook(2, student2);
    lib.issueBook(2, student3);

    cout << "\nLibrary contents after issuing books:" << endl;
    lib.displayAllBooks();
    cout << endl;

    lib.displayIssueQueue();
    cout << endl;
}
void bookReturnOperation(Library& lib) {
    lib.returnBook(1);

    cout << "\nLibrary contents after returning a book:" << endl;
    lib.displayAllBooks();
    cout << endl;

    lib.displayIssueQueue();
    cout << endl;
}
void diaplayAllBookOperation(Library& lib) {
    lib.displayAllBooks();
}

void deletionBookOperation(Library& lib) {
    // Deletion of a Book:
    int idToDelete;
    cout << "Enter the id of the book you want to delete: ";
    cin >> idToDelete;

    if(lib.deleteBook(idToDelete)) {
        cout << "Book with Id " << idToDelete << " deleted successfully." << endl;
        cout << endl;
        cout << "Displaying the list of book in the library after deletion operation: " << endl;
        lib.displayAllBooks();
    } else {
        cout << "Failed to delete book with this id as it doesn't exist." << endl;
    }
}

int main() {
    Library lib;

    cout << "-----------------Library Management System------------------" << endl << endl;

    int choice;
    
    do {
        cout << "Enter 1 for book addition(1)" << endl << "Enter 2 to search a book(2)" << endl << "Enter 3 to issue a book(3)" << endl << "Enter 4 to return a book(4)" << endl << "Enter 5 to display all the books(5)" << endl << "Enter 6 to delete a book(6)" << endl;
        cin >> choice;
        cout << endl;

        switch(choice) {
            case 1:
                bookAdditionOperation(lib);
                cout << endl;
                break;
            case 2:
                bookSearchOperation(lib);
                cout << endl;
                break;
            case 3:
                bookIssueOperation(lib);
                cout << endl;
                break;
            case 4:
                bookReturnOperation(lib);
                cout << endl;
                break;
            case 5:
                diaplayAllBookOperation(lib);
                cout << endl;
                break;
            case 6:
                deletionBookOperation(lib);
                cout << endl;
                break;
            case -1:
                cout << "Exiting the program. Goodbye!" << endl;
            default:
                cout << "Please choose the correct option from the given choice!!!";
        }
    } while(choice != -1);

    return 0;
}
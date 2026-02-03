#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <ctime>

using namespace std;

// Book class to store book information
class Book {
private:
    int bookID;
    string title;
    string author;
    string isbn;
    bool isAvailable;
    int borrowedByMemberID;

public:
    Book(int id, string t, string a, string i)
        : bookID(id), title(t), author(a), isbn(i), isAvailable(true), borrowedByMemberID(-1) {}

    // Getters
    int getBookID() const { return bookID; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getISBN() const { return isbn; }
    bool getAvailability() const { return isAvailable; }
    int getBorrowedBy() const { return borrowedByMemberID; }

    // Setters
    void setAvailability(bool status) { isAvailable = status; }
    void setBorrowedBy(int memberID) { borrowedByMemberID = memberID; }

    // Display book information
    void displayBook() const {
        cout << left << setw(8) << bookID
             << setw(30) << title
             << setw(20) << author
             << setw(15) << isbn
             << setw(12) << (isAvailable ? "Available" : "Borrowed") << endl;
    }
};

// Member class to store member information
class Member {
private:
    int memberID;
    string name;
    string email;
    string phone;
    vector<int> borrowedBooks;

public:
    Member(int id, string n, string e, string p)
        : memberID(id), name(n), email(e), phone(p) {}

    // Getters
    int getMemberID() const { return memberID; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    vector<int> getBorrowedBooks() const { return borrowedBooks; }

    // Add borrowed book
    void borrowBook(int bookID) {
        borrowedBooks.push_back(bookID);
    }

    // Return borrowed book
    void returnBook(int bookID) {
        auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookID);
        if (it != borrowedBooks.end()) {
            borrowedBooks.erase(it);
        }
    }

    // Display member information
    void displayMember() const {
        cout << left << setw(10) << memberID
             << setw(25) << name
             << setw(30) << email
             << setw(15) << phone
             << setw(10) << borrowedBooks.size() << endl;
    }
};

// Library class to manage books and members
class Library {
private:
    vector<Book> books;
    vector<Member> members;
    int nextBookID;
    int nextMemberID;

public:
    Library() : nextBookID(1001), nextMemberID(2001) {}

    // Add a new book
    void addBook() {
        string title, author, isbn;
        cout << "\n--- Add New Book ---" << endl;
        cin.ignore();
        cout << "Enter Book Title: ";
        getline(cin, title);
        cout << "Enter Author Name: ";
        getline(cin, author);
        cout << "Enter ISBN: ";
        getline(cin, isbn);

        books.push_back(Book(nextBookID++, title, author, isbn));
        cout << "\nBook added successfully! Book ID: " << (nextBookID - 1) << endl;
    }

    // Display all books
    void displayAllBooks() const {
        if (books.empty()) {
            cout << "\nNo books in the library." << endl;
            return;
        }

        cout << "\n--- Library Books ---" << endl;
        cout << left << setw(8) << "ID"
             << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(15) << "ISBN"
             << setw(12) << "Status" << endl;
        cout << string(85, '-') << endl;

        for (const auto& book : books) {
            book.displayBook();
        }
    }

    // Search book by title or author
    void searchBook() const {
        string query;
        cin.ignore();
        cout << "\nEnter book title or author to search: ";
        getline(cin, query);

        // Convert query to lowercase for case-insensitive search
        transform(query.begin(), query.end(), query.begin(), ::tolower);

        bool found = false;
        cout << "\n--- Search Results ---" << endl;
        cout << left << setw(8) << "ID"
             << setw(30) << "Title"
             << setw(20) << "Author"
             << setw(15) << "ISBN"
             << setw(12) << "Status" << endl;
        cout << string(85, '-') << endl;

        for (const auto& book : books) {
            string title = book.getTitle();
            string author = book.getAuthor();
            transform(title.begin(), title.end(), title.begin(), ::tolower);
            transform(author.begin(), author.end(), author.begin(), ::tolower);

            if (title.find(query) != string::npos || author.find(query) != string::npos) {
                book.displayBook();
                found = true;
            }
        }

        if (!found) {
            cout << "No books found matching your search." << endl;
        }
    }

    // Add a new member
    void addMember() {
        string name, email, phone;
        cout << "\n--- Add New Member ---" << endl;
        cin.ignore();
        cout << "Enter Member Name: ";
        getline(cin, name);
        cout << "Enter Email: ";
        getline(cin, email);
        cout << "Enter Phone: ";
        getline(cin, phone);

        members.push_back(Member(nextMemberID++, name, email, phone));
        cout << "\nMember added successfully! Member ID: " << (nextMemberID - 1) << endl;
    }

    // Display all members
    void displayAllMembers() const {
        if (members.empty()) {
            cout << "\nNo members registered." << endl;
            return;
        }

        cout << "\n--- Library Members ---" << endl;
        cout << left << setw(10) << "Member ID"
             << setw(25) << "Name"
             << setw(30) << "Email"
             << setw(15) << "Phone"
             << setw(10) << "Books" << endl;
        cout << string(90, '-') << endl;

        for (const auto& member : members) {
            member.displayMember();
        }
    }

    // Issue a book to a member
    void issueBook() {
        int bookID, memberID;
        cout << "\n--- Issue Book ---" << endl;
        cout << "Enter Book ID: ";
        cin >> bookID;
        cout << "Enter Member ID: ";
        cin >> memberID;

        // Find book
        auto bookIt = find_if(books.begin(), books.end(),
            [bookID](const Book& b) { return b.getBookID() == bookID; });

        // Find member
        auto memberIt = find_if(members.begin(), members.end(),
            [memberID](const Member& m) { return m.getMemberID() == memberID; });

        if (bookIt == books.end()) {
            cout << "\nError: Book not found!" << endl;
            return;
        }

        if (memberIt == members.end()) {
            cout << "\nError: Member not found!" << endl;
            return;
        }

        if (!bookIt->getAvailability()) {
            cout << "\nError: Book is already borrowed!" << endl;
            return;
        }

        // Issue the book
        bookIt->setAvailability(false);
        bookIt->setBorrowedBy(memberID);
        memberIt->borrowBook(bookID);

        cout << "\nBook issued successfully!" << endl;
        cout << "Book: " << bookIt->getTitle() << endl;
        cout << "Member: " << memberIt->getName() << endl;
    }

    // Return a book
    void returnBook() {
        int bookID;
        cout << "\n--- Return Book ---" << endl;
        cout << "Enter Book ID: ";
        cin >> bookID;

        // Find book
        auto bookIt = find_if(books.begin(), books.end(),
            [bookID](const Book& b) { return b.getBookID() == bookID; });

        if (bookIt == books.end()) {
            cout << "\nError: Book not found!" << endl;
            return;
        }

        if (bookIt->getAvailability()) {
            cout << "\nError: This book is not currently borrowed!" << endl;
            return;
        }

        int memberID = bookIt->getBorrowedBy();

        // Find member
        auto memberIt = find_if(members.begin(), members.end(),
            [memberID](const Member& m) { return m.getMemberID() == memberID; });

        // Return the book
        bookIt->setAvailability(true);
        bookIt->setBorrowedBy(-1);
        if (memberIt != members.end()) {
            memberIt->returnBook(bookID);
        }

        cout << "\nBook returned successfully!" << endl;
        cout << "Book: " << bookIt->getTitle() << endl;
    }

    // Display borrowed books by a member
    void displayMemberBooks() const {
        int memberID;
        cout << "\nEnter Member ID: ";
        cin >> memberID;

        auto memberIt = find_if(members.begin(), members.end(),
            [memberID](const Member& m) { return m.getMemberID() == memberID; });

        if (memberIt == members.end()) {
            cout << "\nError: Member not found!" << endl;
            return;
        }

        cout << "\n--- Books Borrowed by " << memberIt->getName() << " ---" << endl;
        vector<int> borrowedBooks = memberIt->getBorrowedBooks();

        if (borrowedBooks.empty()) {
            cout << "No books currently borrowed." << endl;
            return;
        }

        cout << left << setw(8) << "ID"
             << setw(30) << "Title"
             << setw(20) << "Author" << endl;
        cout << string(58, '-') << endl;

        for (int bookID : borrowedBooks) {
            auto bookIt = find_if(books.begin(), books.end(),
                [bookID](const Book& b) { return b.getBookID() == bookID; });

            if (bookIt != books.end()) {
                cout << left << setw(8) << bookIt->getBookID()
                     << setw(30) << bookIt->getTitle()
                     << setw(20) << bookIt->getAuthor() << endl;
            }
        }
    }
};

// Main menu function
void displayMenu() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   LIBRARY MANAGEMENT SYSTEM            ║" << endl;
    cout << "╠════════════════════════════════════════╣" << endl;
    cout << "║  1. Add Book                           ║" << endl;
    cout << "║  2. Display All Books                  ║" << endl;
    cout << "║  3. Search Book                        ║" << endl;
    cout << "║  4. Add Member                         ║" << endl;
    cout << "║  5. Display All Members                ║" << endl;
    cout << "║  6. Issue Book                         ║" << endl;
    cout << "║  7. Return Book                        ║" << endl;
    cout << "║  8. Display Member's Books             ║" << endl;
    cout << "║  9. Exit                               ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "Enter your choice: ";
}

int main() {
    Library library;
    int choice;

    // Add some sample data
    cout << "Initializing Library Management System...\n" << endl;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                library.addBook();
                break;
            case 2:
                library.displayAllBooks();
                break;
            case 3:
                library.searchBook();
                break;
            case 4:
                library.addMember();
                break;
            case 5:
                library.displayAllMembers();
                break;
            case 6:
                library.issueBook();
                break;
            case 7:
                library.returnBook();
                break;
            case 8:
                library.displayMemberBooks();
                break;
            case 9:
                cout << "\nThank you for using Library Management System!" << endl;
                return 0;
            default:
                cout << "\nInvalid choice! Please try again." << endl;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }

    return 0;
}

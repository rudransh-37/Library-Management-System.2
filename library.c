#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BOOKS 100
#define MAX_MEMBERS 100
#define MAX_ISSUES 500
#define MAX_LENGTH 100
#define FINE_PER_DAY 5

#define BOOK_FILE "books.txt"
#define MEMBER_FILE "members.txt"
#define ISSUE_FILE "issues.txt"

// ==================== STRUCTURES ====================

struct Book {
    int id;
    char title[MAX_LENGTH];
    char author[MAX_LENGTH];
    char isbn[MAX_LENGTH];
    int totalCopies;
    int availableCopies;
};

struct Member {
    int id;
    char name[MAX_LENGTH];
    char department[MAX_LENGTH];
    char phone[MAX_LENGTH];
};

struct Issue {
    int bookId;
    int memberId;
    int allowedDays;
    int actualDays;
    int returned;
};

// ==================== GLOBAL ARRAYS ====================

struct Book books[MAX_BOOKS];
struct Member members[MAX_MEMBERS];
struct Issue issues[MAX_ISSUES];

int bookCount = 0;
int memberCount = 0;
int issueCount = 0;

// ==================== INPUT FUNCTIONS ====================

// Read an integer safely
int readInt(const char message[]) {
    int value;
    int result;
    int ch;

    while (1) {
        printf("%s", message);
        result = scanf("%d", &value);

        if (result == 1) {
            while ((ch = getchar()) != '\n' && ch != EOF);
            return value;
        }

        if (result == EOF) {
            printf("\nInput ended. Exiting program.\n");
            exit(0);
        }

        printf("Invalid input! Enter a number.\n");

        while ((ch = getchar()) != '\n' && ch != EOF);
    }
}

// Read a complete line
void readLine(const char message[], char input[], int size) {
    size_t len;
    int ch;

    printf("%s", message);

    if (fgets(input, size, stdin) == NULL) {
        printf("\nInput ended. Exiting program.\n");
        exit(0);
    }

    len = strlen(input);

    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF);
    }
}

// ==================== SEARCH FUNCTIONS ====================

// Find book by ID using linear search
int findBook(int id) {
    int i;

    for (i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            return i;
        }
    }

    return -1;
}

// Find member by ID using linear search
int findMember(int id) {
    int i;

    for (i = 0; i < memberCount; i++) {
        if (members[i].id == id) {
            return i;
        }
    }

    return -1;
}

// ==================== FILE HANDLING ====================

// Save all book records
int saveBooks(void) {
    FILE *fp;
    int i;

    fp = fopen(BOOK_FILE, "w");

    if (fp == NULL) {
        printf("Error saving books!\n");
        return 0;
    }

    for (i = 0; i < bookCount; i++) {
        if (fprintf(fp, "%d|%s|%s|%s|%d|%d\n",
                    books[i].id,
                    books[i].title,
                    books[i].author,
                    books[i].isbn,
                    books[i].totalCopies,
                    books[i].availableCopies) < 0) {
            fclose(fp);
            printf("Error writing book records!\n");
            return 0;
        }
    }

    if (fclose(fp) != 0) {
        printf("Error closing books file!\n");
        return 0;
    }

    return 1;
}

// Load book records
void loadBooks(void) {
    FILE *fp;
    struct Book temp;
    int count = 0;

    fp = fopen(BOOK_FILE, "r");

    if (fp == NULL) {
        return;
    }

    while (count < MAX_BOOKS &&
           fscanf(fp, "%d|%99[^|]|%99[^|]|%99[^|]|%d|%d",
                  &temp.id,
                  temp.title,
                  temp.author,
                  temp.isbn,
                  &temp.totalCopies,
                  &temp.availableCopies) == 6) {

        if (temp.id <= 0 ||
            temp.totalCopies <= 0 ||
            temp.availableCopies < 0 ||
            temp.availableCopies > temp.totalCopies) {
            printf("Invalid book record found. Loading stopped.\n");
            break;
        }

        books[count] = temp;
        count++;
    }

    bookCount = count;
    fclose(fp);
}

// Save member records
int saveMembers(void) {
    FILE *fp;
    int i;

    fp = fopen(MEMBER_FILE, "w");

    if (fp == NULL) {
        printf("Error saving members!\n");
        return 0;
    }

    for (i = 0; i < memberCount; i++) {
        if (fprintf(fp, "%d|%s|%s|%s\n",
                    members[i].id,
                    members[i].name,
                    members[i].department,
                    members[i].phone) < 0) {
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0) {
        printf("Error closing members file!\n");
        return 0;
    }

    return 1;
}

// Load member records
void loadMembers(void) {
    FILE *fp;
    struct Member temp;
    int count = 0;

    fp = fopen(MEMBER_FILE, "r");

    if (fp == NULL) {
        return;
    }

    while (count < MAX_MEMBERS &&
           fscanf(fp, "%d|%99[^|]|%99[^|]|%99[^\n]",
                  &temp.id,
                  temp.name,
                  temp.department,
                  temp.phone) == 4) {

        if (temp.id <= 0) {
            printf("Invalid member record found. Loading stopped.\n");
            break;
        }

        members[count] = temp;
        count++;
    }

    memberCount = count;
    fclose(fp);
}

// Save issue records
int saveIssues(void) {
    FILE *fp;
    int i;

    fp = fopen(ISSUE_FILE, "w");

    if (fp == NULL) {
        printf("Error saving issue records!\n");
        return 0;
    }

    for (i = 0; i < issueCount; i++) {
        if (fprintf(fp, "%d|%d|%d|%d|%d\n",
                    issues[i].bookId,
                    issues[i].memberId,
                    issues[i].allowedDays,
                    issues[i].actualDays,
                    issues[i].returned) < 0) {
            fclose(fp);
            return 0;
        }
    }

    if (fclose(fp) != 0) {
        printf("Error closing issue file!\n");
        return 0;
    }

    return 1;
}

// Load issue records
void loadIssues(void) {
    FILE *fp;
    struct Issue temp;
    int count = 0;

    fp = fopen(ISSUE_FILE, "r");

    if (fp == NULL) {
        return;
    }

    while (count < MAX_ISSUES &&
           fscanf(fp, "%d|%d|%d|%d|%d",
                  &temp.bookId,
                  &temp.memberId,
                  &temp.allowedDays,
                  &temp.actualDays,
                  &temp.returned) == 5) {

        if (temp.allowedDays <= 0 ||
            temp.actualDays < 0 ||
            (temp.returned != 0 && temp.returned != 1)) {
            printf("Invalid issue record found. Loading stopped.\n");
            break;
        }

        issues[count] = temp;
        count++;
    }

    issueCount = count;
    fclose(fp);
}

// ==================== BOOK MANAGEMENT ====================

// Add a new book
void addBook(void) {
    struct Book b;
    int i;

    if (bookCount >= MAX_BOOKS) {
        printf("Book storage is full!\n");
        return;
    }

    b.id = readInt("\nEnter Book ID: ");

    if (b.id <= 0 || findBook(b.id) != -1) {
        printf("Invalid or duplicate Book ID!\n");
        return;
    }

    readLine("Enter book title: ", b.title, MAX_LENGTH);
    readLine("Enter author name: ", b.author, MAX_LENGTH);
    readLine("Enter ISBN: ", b.isbn, MAX_LENGTH);

    if (strlen(b.title) == 0 ||
        strlen(b.author) == 0 ||
        strlen(b.isbn) == 0 ||
        strchr(b.title, '|') != NULL ||
        strchr(b.author, '|') != NULL ||
        strchr(b.isbn, '|') != NULL) {
        printf("Invalid or empty book details!\n");
        return;
    }

    for (i = 0; i < bookCount; i++) {
        if (strcmp(books[i].isbn, b.isbn) == 0) {
            printf("This ISBN already exists!\n");
            return;
        }
    }

    b.totalCopies = readInt("Enter total copies: ");

    if (b.totalCopies <= 0) {
        printf("Copies must be greater than zero!\n");
        return;
    }

    b.availableCopies = b.totalCopies;

    books[bookCount] = b;
    bookCount++;

    if (!saveBooks()) {
        bookCount--;
        printf("Could not save book. Addition cancelled.\n");
        return;
    }

    printf("Book added successfully!\n");
}

// Display all books
void displayBooks(void) {
    int i;

    if (bookCount == 0) {
        printf("\nNo books available.\n");
        return;
    }

    printf("\n========== BOOK LIST ==========\n");

    for (i = 0; i < bookCount; i++) {
        printf("\nBook ID: %d", books[i].id);
        printf("\nTitle: %s", books[i].title);
        printf("\nAuthor: %s", books[i].author);
        printf("\nISBN: %s", books[i].isbn);
        printf("\nTotal Copies: %d", books[i].totalCopies);
        printf("\nAvailable Copies: %d\n",
               books[i].availableCopies);
        printf("-------------------------------\n");
    }
}

// Search books by ID, title, author, or ISBN
void searchBook(void) {
    char query[MAX_LENGTH];
    int id, index, i, found = 0;

    printf("\n1. Search by Book ID\n");
    printf("2. Search by Title\n");
    printf("3. Search by Author\n");
    printf("4. Search by ISBN\n");

    id = readInt("Enter search option: ");

    if (id == 1) {
        int bookId = readInt("Enter Book ID: ");

        index = findBook(bookId);

        if (index != -1) {
            printf("\nTitle: %s\nAuthor: %s\nISBN: %s\n",
                   books[index].title,
                   books[index].author,
                   books[index].isbn);
            printf("Available Copies: %d\n",
                   books[index].availableCopies);
        } else {
            printf("Book not found!\n");
        }

        return;
    }

    if (id < 2 || id > 4) {
        printf("Invalid search option!\n");
        return;
    }

    readLine("Enter search text: ", query, MAX_LENGTH);

    for (i = 0; i < bookCount; i++) {
        if ((id == 2 && strcmp(books[i].title, query) == 0) ||
            (id == 3 && strcmp(books[i].author, query) == 0) ||
            (id == 4 && strcmp(books[i].isbn, query) == 0)) {

            printf("\nBook ID: %d\nTitle: %s\nAuthor: %s\n",
                   books[i].id,
                   books[i].title,
                   books[i].author);

            printf("ISBN: %s\nAvailable Copies: %d\n",
                   books[i].isbn,
                   books[i].availableCopies);

            found = 1;
        }
    }

    if (!found) {
        printf("Book not found!\n");
    }
}

// ==================== MEMBER MANAGEMENT ====================

// Add a member
void addMember(void) {
    struct Member m;

    if (memberCount >= MAX_MEMBERS) {
        printf("Member storage is full!\n");
        return;
    }

    m.id = readInt("\nEnter Member ID: ");

    if (m.id <= 0 || findMember(m.id) != -1) {
        printf("Invalid or duplicate Member ID!\n");
        return;
    }

    readLine("Enter member name: ", m.name, MAX_LENGTH);
    readLine("Enter department: ", m.department, MAX_LENGTH);
    readLine("Enter phone number: ", m.phone, MAX_LENGTH);

    if (strlen(m.name) == 0 ||
        strlen(m.department) == 0 ||
        strlen(m.phone) == 0 ||
        strchr(m.name, '|') != NULL ||
        strchr(m.department, '|') != NULL ||
        strchr(m.phone, '|') != NULL) {
        printf("Invalid member details!\n");
        return;
    }

    members[memberCount] = m;
    memberCount++;

    if (!saveMembers()) {
        memberCount--;
        printf("Could not save member. Addition cancelled.\n");
        return;
    }

    printf("Member registered successfully!\n");
}

// Display all members
void displayMembers(void) {
    int i;

    if (memberCount == 0) {
        printf("No members registered.\n");
        return;
    }

    printf("\n========== MEMBER LIST ==========\n");

    for (i = 0; i < memberCount; i++) {
        printf("\nMember ID: %d", members[i].id);
        printf("\nName: %s", members[i].name);
        printf("\nDepartment: %s", members[i].department);
        printf("\nPhone: %s\n", members[i].phone);
        printf("---------------------------------\n");
    }
}

// ==================== ISSUE MANAGEMENT ====================

// Issue a book
void issueBook(void) {
    int bookId, memberId, days;
    int bi, mi;
    struct Issue issue;

    if (issueCount >= MAX_ISSUES) {
        printf("Issue record storage is full!\n");
        return;
    }

    bookId = readInt("\nEnter Book ID: ");
    bi = findBook(bookId);

    if (bi == -1) {
        printf("Book not found!\n");
        return;
    }

    if (books[bi].availableCopies <= 0) {
        printf("No copies available!\n");
        return;
    }

    memberId = readInt("Enter Member ID: ");
    mi = findMember(memberId);

    if (mi == -1) {
        printf("Member not found!\n");
        return;
    }

    days = readInt("Enter allowed borrowing days: ");

    if (days <= 0) {
        printf("Borrowing days must be positive!\n");
        return;
    }

    issue.bookId = bookId;
    issue.memberId = memberId;
    issue.allowedDays = days;
    issue.actualDays = 0;
    issue.returned = 0;

    books[bi].availableCopies--;
    issues[issueCount] = issue;
    issueCount++;

    if (!saveBooks() || !saveIssues()) {
        books[bi].availableCopies++;
        issueCount--;

        printf("Could not save issue records. Please check the files.\n");
        return;
    }

    printf("Book issued successfully!\n");
}

// Return a book
void returnBook(void) {
    int bookId, memberId;
    int bi, i, days, fine;
    int found = 0;

    bookId = readInt("\nEnter Book ID: ");
    memberId = readInt("Enter Member ID: ");

    bi = findBook(bookId);

    if (bi == -1) {
        printf("Book not found!\n");
        return;
    }

    // Find the matching active borrowing record
    for (i = 0; i < issueCount; i++) {
        if (issues[i].bookId == bookId &&
            issues[i].memberId == memberId &&
            issues[i].returned == 0) {

            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No active issue record found for this member and book.\n");
        return;
    }

    days = readInt("Enter actual days the book was kept: ");

    if (days < 0) {
        printf("Days cannot be negative!\n");
        return;
    }

    if (days > issues[i].allowedDays) {
        fine = (days - issues[i].allowedDays) * FINE_PER_DAY;
    } else {
        fine = 0;
    }

    // Save previous values for rollback on a reported save failure
    {
        int oldAvailable = books[bi].availableCopies;
        int oldReturned = issues[i].returned;
        int oldActualDays = issues[i].actualDays;

        books[bi].availableCopies++;
        issues[i].actualDays = days;
        issues[i].returned = 1;

        if (!saveBooks() || !saveIssues()) {
            books[bi].availableCopies = oldAvailable;
            issues[i].returned = oldReturned;
            issues[i].actualDays = oldActualDays;

            printf("Could not save return records. Please check the files.\n");
            return;
        }
    }

    printf("Book returned successfully!\n");
    printf("Fine amount: Rs. %d\n", fine);
}

// Display stock report
void stockReport(void) {
    int i;
    int total = 0;
    int available = 0;

    for (i = 0; i < bookCount; i++) {
        total += books[i].totalCopies;
        available += books[i].availableCopies;
    }

    printf("\nTotal book copies: %d\n", total);
    printf("Available copies: %d\n", available);
    printf("Issued copies: %d\n", total - available);
}

// ==================== MAIN FUNCTION ====================

int main(void) {
    int choice;

    loadBooks();
    loadMembers();
    loadIssues();

    do {
        printf("\n====================================\n");
        printf("     LIBRARY MANAGEMENT SYSTEM\n");
        printf("====================================\n");
        printf("1. Add Book\n");
        printf("2. Display All Books\n");
        printf("3. Search Book\n");
        printf("4. Register Member\n");
        printf("5. Display Members\n");
        printf("6. Issue Book\n");
        printf("7. Return Book\n");
        printf("8. Stock Report\n");
        printf("9. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addBook();
                break;

            case 2:
                displayBooks();
                break;

            case 3:
                searchBook();
                break;

            case 4:
                addMember();
                break;

            case 5:
                displayMembers();
                break;

            case 6:
                issueBook();
                break;

            case 7:
                returnBook();
                break;

            case 8:
                stockReport();
                break;

            case 9:
                printf("Thank you for using the Library Management System!\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 9);

    return 0;
}
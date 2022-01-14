/** This class represents a library, which hold a collection of books.
 * Patrons can register at the library to be able to check out books,
 * if a copy of the requested book is available.*/

public class Library {

    /* number of books that can be registered within a library*/
    int maxBooks;
    
    /* array containing all added books */
    Book[] bookArray;

    /* number of books that can be borrowed by patrons */
    int borrowedCapacity;

    /* number of patrons that can be registered to library */
    int maxPatrons;
    
    /*array containing all registered patrons */
    Patron[] patronArray;

    /** constructor
     * Creates a new library with the given parameters.
     * @param maxBookCapacity- the maximum capacity of books within library
     * @param maxBorrowedBooks- the maximum capacity of borrowed books per patron
     * @param maxPatronCapacity- the maximum capacity of patrons that can be registered
      */

    Library(int maxBookCapacity, int maxBorrowedBooks, int maxPatronCapacity){
        maxBooks = maxBorrowedBooks;
        bookArray = new Book[maxBooks]; //initial array of books is empty
        borrowedCapacity = maxBorrowedBooks;
        maxPatrons = maxPatronCapacity;
        patronArray = new Patron[maxPatrons]; //initial array of patrons is empty
    }

    int addBookToLibrary(Book book){
        for (int i = 0; i < maxBooks; i++) {
            if (bookArray[i] instanceof Book) {
                if (bookArray[i] == book) {
                    return i;
                }
            } else {
                bookArray[i] = book;
                return i;
            }
        }
        // book won't be added if array is full
        return -1;
    }

    boolean isBookIdValid(int bookId){
        if (bookId < maxBooks) {
            return (bookArray[bookId] instanceof Book);
        } else {
            return false;
        }

    }

    int getBookId(Book book) {
        for (int i = 0; i < maxBooks; i++){
            if(bookArray[i] instanceof Book){
                if (bookArray[i] == book){
                    return i;
                }
            } else {
                break;
                //if we've reached empty array entries stop searching
            }
        }
        return -1;
    }


    boolean isBookAvailable(int bookId){
        if (isBookIdValid(bookId)) {
            currentBorrowerId = bookArray[bookId].getCurrentBorrowerId;
            return (currentBorrowerId < 0);
        }
    }

    int registerPatronToLibrary(Patron patron){
        // don't register if patronArray is full
        for (int i = 0; i < maxPatrons; i++) {
            if (patronArray[i] instanceof Patron) {
                if (patronArray[i] == patron) {
                    return i;
                }
            } else {
                patronArray[i] = patron;
                return i;
            }
        }
        // patrons won't be added if array is full
        return -1;
    }

    boolean isPatronIdValid(int patronId){
        if (patronId < maxPatrons) {
            return (patronArray[patronId] instanceof Patron);
        } else {
            return false;
        }
    }

    int getPatronId(Patron patron){
        for (int i = 0; i < maxPatrons; i++){
            if(patronArray[i] instanceof Patron){
                if (patronArray[i] == patron){
                    return i;
                }
            } else {
                break;
                //if we've reached empty array entries stop searching
            }
        }
        return -1;
    }

    boolean borrowBook(int bookId,
                       int patronId){
        if (isBookAvailable(bookId) && isPatronIdValid(patronId)){
            //check how many books were borrowed by patron
            int booksBorrowed = 0;
            for (int i = 0; i < maxBooks; i++){
                if (bookArray[i] instanceof Book) {
                    if (bookArray[i].getCurrentBorrowerId == patronId) {
                        booksBorrowed++;
                    }
                } else {
                    break;
                }
            if (booksBorrowed < borrowedCapacity) {
                bookArray[bookId].setBorrowerId(patronId);
                return true;
            }
        }
        return false;
    }


    }

    void returnBook(int bookId){
        if (isBookIdValid(bookId)){
            bookArray[bookId].returnBook();
        }
    }

    Book suggestBookToPatron(int patronId){
        // book must be available
        Book mostEnjoy = null;
        if (isPatronIdValid(patronId)) {
            Patron patron = patronArray[patronId];
            int highestScore = 0;
            for (int i = 0; i < maxBooks; i++){
                if (bookArray[i] instanceof Book){
                    if (bookArray[i].isBookAvailable && patron.willEnjoyBook(bookArray[i])){
                        int currentScore = patron.getBookScore(bookArray[i]);
                        if (currentScore > highestScore){
                        mostEnjoy = bookArray[i];
                        highestScore = currentScore;
                        }
                    }
                } else {
                    break;
                }
            }
        }
        return mostEnjoy;
    }

}




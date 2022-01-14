/** This class represents a library patron that has a name and assigns values to different literary aspects of books.
 * to different literary aspects of books. */

public class Patron {

    /** The first name of this patron */
    final String first;

    /** The last name of this patron. */
    final String last;

    /**The comic tendency of this patron. */
    int comic;

    /** The dramatic value of this patron. */
    int dramatic;

    /** The educational value of this patron. */
    int educational;

    /** the threshold of this patron. */
    int thresh;


    /*----=  Constructor  =-----*/

    /**
     * Creates a new PATRON with the given characteristic.
     *
     * @param patronFirstName          The first name of the patron.
     * @param bookAuthor               The last name of the patron.
     * @param comicTendency            The comic tendency of the patron.
     * @param dramaticTendency         The dramatic tendency of the patron.
     * @param educationalTendency      The educational tendency of the patron.
     * @param patronEnjoymentThreshold the enjoyment threshold of the patron
     */
    Patron(String patronFirstName, String patronLastName, int comicTendency, int dramaticTendency,
           int educationalTendency, int patronEnjoymentThreshold) {
        first = patronFirstName;
        last = patronLastName;
        comic = comicTendency;
        dramatic = dramaticTendency;
        educational = educationalTendency;
        thresh = patronEnjoymentThreshold;
    }

    /* return first and last name of the patron seperated by whitespace */
    String stringRepresentation() {
        return first + "" + last;
    }

    /**
     * Returns the literary value this patron assigns to the given book.
     *
     * @param book- book to assess
     * @returns literary value.
     */
    int getBookScore (Book book) {
        int score = comic * book.comicValue + dramatic * book.dramaticValue +
                educational * book.educationalValue;
        return score;
    }

    /**
     * Returns true of this patron will enjoy the given book, false otherwise.
     *
     * @param book- book to assess
     * @return boolean value
     */
    boolean willEnjoyBook(Book book) {
        return getBookScore(book) > thresh;
    }
}



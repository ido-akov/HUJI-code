package filesprocessing;

/**
 * exception to be thrown in case of a type two error
 */
public class TypeTwoException extends RuntimeException {

    private static final long serialVersionUID = 1L;
    private static final String TYPE_TWO_ERROR = "ERROR: ";

    //constructor
    public TypeTwoException(String error) {
        super(TYPE_TWO_ERROR + error);
    }
}

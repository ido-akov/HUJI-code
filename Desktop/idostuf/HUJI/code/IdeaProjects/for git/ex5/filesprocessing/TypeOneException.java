package filesprocessing;

/**
 * exception to be thrown in case of a type one error
 */
public class TypeOneException extends Exception {

    private static final long serialVersionUID = 1L;
    private static final String TYPE_ONE_ERROR = "Warning in line ";

//constructor
    public TypeOneException() {
        super(TYPE_ONE_ERROR);
    }
}

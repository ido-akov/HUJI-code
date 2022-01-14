package filesOrdering;
import java.io.File;
import java.util.Comparator;

/**
 * child class of byOrder, this object sorts a file array by absolute path
 */
public class ByAbsolutePath extends ByOrder {

    //comparator by absolute path
    private static final Comparator<File> COMPARE_BY_PATH = Comparator.comparing(File::getAbsolutePath);

    //constructor
    public ByAbsolutePath(boolean isReversed) {
        super(COMPARE_BY_PATH, isReversed);
    }

    /**
     * default constructor for empty or invalid input
     */
    public ByAbsolutePath() {
        super(COMPARE_BY_PATH, false);
    }
}

package filesOrdering;
import java.io.File;
import java.util.Comparator;

/**
 * child class of byOrder, this object sorts files by their size
 */
public class BySize extends ByOrder {

    //comparator by file size
    private static final Comparator<File> COMPARE_BY_SIZE = new Comparator<File>() {
        @Override
        public int compare(File file1, File file2) {
            int compareVal = Double.compare(file1.length(), file2.length());
            if (compareVal == 0) {
                return file1.getAbsolutePath().compareTo(file2.getAbsolutePath());
            } else {
                return compareVal;
            }
        }
    };

    public BySize(boolean isReversed) {
        //anonymous comparator by file size
        super(COMPARE_BY_SIZE, isReversed);
    }

}

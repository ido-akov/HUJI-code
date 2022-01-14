package filesOrdering;
import java.io.File;
import java.util.Comparator;

/**
 * child class of byOrder, this object sorts files by their type
 */
public class ByFileType extends ByOrder{

    private static final String TOKEN= "[.]";

    //anonymous comparator by file type
    private static final Comparator<File> COMPARE_BY_TYPE = new Comparator<File>() {
        @Override
        public int compare(File file1, File file2) {
                String type1 = getType(file1);
                String type2 = getType(file2);
                int compareVal =  type1.compareTo(type2);
                if (compareVal == 0) {
                    //compare by absolute path
                    return file1.getAbsolutePath().compareTo(file2.getAbsolutePath());
                } else {
                    return compareVal;
                }
        }
    };
    /**
     * constructor. Uses parent constructor with comparator by file types.
     */
    public ByFileType(boolean isReversed) {
        super(COMPARE_BY_TYPE, isReversed);
    }

    /**
     * this method gets the type of a file by checking its suffix.
     * @param file who's type we are searching for
     * @return file type
     */
    private static String getType(File file) {
        String name = file.getName();
        String[] split = name.split(TOKEN);
        if (split.length <= 1) {
            //if file doesn't have a type
            return "";
        } else {
            //this is file type
            return split[split.length - 1];
        }
    }
}

package filesFiltering;
import java.io.File;
import java.io.FileFilter;

//FileFilter which returns files of size smaller than a given double
public class smaller_thanFileFilter implements FileFilter {
    private double sizeRestriction;
    private boolean satisfies;

    //constructor
    public smaller_thanFileFilter(double sizeRestriction, boolean satisfies) {
        this.sizeRestriction = sizeRestriction;
        this.satisfies = satisfies;
    }

    /**
     * converts byte size to kilobytes
     * @param bytes file size
     * @return size in kilobytes
     */
    private double toKiloBytes (long bytes) {
        return (double) bytes / 1024;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return (toKiloBytes(file.length()) < sizeRestriction) == satisfies;
    }
}



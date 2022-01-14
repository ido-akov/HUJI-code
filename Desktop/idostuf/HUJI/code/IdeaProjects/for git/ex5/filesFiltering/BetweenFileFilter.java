package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns all files in a given range
public class BetweenFileFilter implements FileFilter {

    private double lower;
    private double higher;
    private boolean satisfies;

    //constructor
    public BetweenFileFilter(double lower, double higher, boolean satisfies) {
        this.lower = lower;
        this.higher = higher;
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
        double sizeInKiloBytes = toKiloBytes(file.length());
        return (lower<= sizeInKiloBytes && sizeInKiloBytes <= higher) == satisfies;
    }
}

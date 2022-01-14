package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns files who's prefix is a given string
public class prefixFileFilter implements FileFilter {

    private String isPrefix;
    private boolean satisfies;

    //constructor
    public prefixFileFilter(String isPrefix, boolean satisfies) {
        this.isPrefix = isPrefix;
        this.satisfies = satisfies;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return file.getName().startsWith(isPrefix) == satisfies;
    }
}

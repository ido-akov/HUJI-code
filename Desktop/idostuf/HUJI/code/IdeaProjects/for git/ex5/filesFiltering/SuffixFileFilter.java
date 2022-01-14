package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns files who's suffix is a given string
public class SuffixFileFilter implements FileFilter {

    private String isSuffix;
    private boolean satisfies;

    //constructor
    public SuffixFileFilter(String isSuffix, boolean satisfies) {
        this.isSuffix = isSuffix;
        this.satisfies = satisfies;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return file.getName().endsWith(isSuffix) == satisfies;
    }
}

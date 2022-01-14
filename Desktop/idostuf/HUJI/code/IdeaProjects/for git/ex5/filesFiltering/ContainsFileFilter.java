package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns files who's name contains a given string
public class ContainsFileFilter implements FileFilter {

    private String isContained;
    private boolean satisfies;

    public ContainsFileFilter(String isContained, boolean satisfies) {
        this.isContained = isContained;
        this.satisfies = satisfies;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return file.getName().contains(isContained) == satisfies;
    }
}

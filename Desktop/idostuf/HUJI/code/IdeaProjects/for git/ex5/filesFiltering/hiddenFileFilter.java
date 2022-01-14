package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns files which are hidden
public class hiddenFileFilter implements FileFilter {

    private boolean condition;
    private boolean satisfies;

    //constructor
    public hiddenFileFilter(boolean condition, boolean satisfies) {
        this.condition = condition;
        this.satisfies = satisfies;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return (file.isHidden() == condition) == satisfies;
    }
}

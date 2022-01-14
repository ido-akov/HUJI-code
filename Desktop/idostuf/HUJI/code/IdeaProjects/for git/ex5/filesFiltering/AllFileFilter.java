package filesFiltering;

import java.io.File;
import java.io.FileFilter;

//FileFilter which returns all files or none according to NOT argument
public class AllFileFilter implements FileFilter {

    private boolean satisfies;
    //constructor
    public AllFileFilter(boolean satisfies) {
        this.satisfies = satisfies;
    }

    /**
     * default constructor for invalid values
     */
    public AllFileFilter() {
        this.satisfies = true;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return satisfies;
    }
}

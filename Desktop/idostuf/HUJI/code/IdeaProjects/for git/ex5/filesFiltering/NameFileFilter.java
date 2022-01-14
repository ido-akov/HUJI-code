package filesFiltering;

import java.io.File;

//FileFilter which returns files who's name is a given string
public class NameFileFilter implements java.io.FileFilter {

    private String name;
    private boolean satisfies;

    //constructor
    public NameFileFilter(String name, boolean satisfies) {
        this.name = name;
        this.satisfies = satisfies;
    }

    /**
     * method which checks if file should be filtered out
     * @param file to be filtered
     * @return if file satisfies condition
     */
    @Override
    public boolean accept(File file) {
        return file.getName().equals(name) == satisfies;
    }
}

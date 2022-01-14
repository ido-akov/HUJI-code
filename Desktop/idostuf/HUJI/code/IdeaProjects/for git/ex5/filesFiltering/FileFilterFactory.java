package filesFiltering;

import filesprocessing.TypeOneException;

import java.io.FileFilter;

//factory for creating new filters
public class FileFilterFactory {

    private static String[] filterParse;
    private static final String TOKEN = "#", NOT = "NOT", YES = "YES", NO = "NO", ALL = "all", BETWEEN = "between",
        CONTAINS = "contains", EXECUTABLE = "executable", FILE = "file", GREATER_THAN = "greater_than",
        SMALLER_THAN = "smaller_than", HIDDEN = "hidden", PREFIX = "prefix", SUFFIX = "suffix", WRITABLE = "writable";

    /**
     * this method attemps to create a new file filter according to input in the commands file
     * @param toParse string to parse
     * @return a new filter
     * @throws TypeOneException in case of invalid input
     */
    public static FileFilter getFileFilter(String toParse) throws TypeOneException {
        filterParse = toParse.split(TOKEN);
        boolean satisfies = checkForNot(filterParse);
        String filterType = filterParse[0];
            switch (filterType) {
                case ALL:
                    checkSize(2);
                    return new AllFileFilter(satisfies);
                case GREATER_THAN:
                    return getGreaterThanFileFilter(satisfies, filterParse[1]);
                case SMALLER_THAN:
                    return getSmallerThanFileFilter(satisfies, filterParse[1]);
                case BETWEEN:
                    return getBetweenFileFilter(satisfies);
                case EXECUTABLE:
                    return new executableFileFilter(getCondition(filterParse[1]), satisfies);
                case WRITABLE:
                    return new writableFileFilter(getCondition(filterParse[1]), satisfies);
                case HIDDEN:
                    return new hiddenFileFilter(getCondition(filterParse[1]), satisfies);
                case FILE:
                    return new NameFileFilter(getStringInName(), satisfies);
                case CONTAINS:
                    return new ContainsFileFilter(getStringInName(), satisfies);
                case PREFIX:
                    return new prefixFileFilter(getStringInName(), satisfies);
                case SUFFIX:
                    return new SuffixFileFilter(getStringInName(), satisfies);
                default:
                    throw new TypeOneException();
            }
        }

    /**
     * check if filter to be returned should filter according to files not satisfying condition
     * @param parsedLine to be parsed
     * @return if input includes not
     */
    private static boolean checkForNot (String[] parsedLine) {
        return !parsedLine[filterParse.length - 1].equals(NOT);
    }


    /**
     * create a new greater_than filter
     * @param satisfies condition to be satisfies
     * @param s value to be parsed
     * @return a new greater_than filter if input values are valid
     * @throws TypeOneException
     */
    private static FileFilter getGreaterThanFileFilter(boolean satisfies, String s) throws TypeOneException {
        checkSize(3);
        try {
            double size = Double.parseDouble(s);
            if (invalidSize(size)) {
                throw new TypeOneException();
            }
            return new greater_thanFileFilter(size, satisfies);
        } catch (NumberFormatException n) {
            throw new TypeOneException();
        }
    }

    /**
     * check if a filter type has the right amount of values
     * @throws TypeOneException
     */
    private static void checkSize(int i) throws TypeOneException {
        if (filterParse.length > i || filterParse.length < i-1 ||
                (filterParse.length == i && !filterParse[i-1].equals(NOT))) {
            throw new TypeOneException();
        }
    }

    /**
     * check if the size value for a size based filter is valid
     * @param size
     * @return
     */
    private static boolean invalidSize (double size) {
        return size < 0;
    }

    /**
     * create a new smaller_than filter
     * @param satisfies condition to be satisfies
     * @param s value to be parsed
     * @return a new smaller_than filter if input values are valid
     * @throws TypeOneException
     */
    private static FileFilter getSmallerThanFileFilter(boolean satisfies, String s) throws TypeOneException {
        checkSize(3);
        try {
            double size = Double.parseDouble(filterParse[1]);
            if (invalidSize(size)) {
                throw new TypeOneException();
            }
            return new smaller_thanFileFilter(size, satisfies);
        } catch (NumberFormatException n) {
            throw new TypeOneException();
        }
    }

    /**
     * create a new between filter
     * @param satisfies condition to be satisfies
     * @return a new smaller_than filter if input values are valid
     * @throws TypeOneException
     */
    private static FileFilter getBetweenFileFilter(boolean satisfies) throws TypeOneException {
        checkSize(4);
        try {
            double size1 = Double.parseDouble(filterParse[1]);
            double size2 = Double.parseDouble(filterParse[2]);
            if (invalidSize(size1) || invalidSize(size2) || invalidRange(size1, size2)) {
                throw new TypeOneException();
            }
            return new BetweenFileFilter(size1, size2, satisfies);
        } catch (NumberFormatException n) {
            throw new TypeOneException();
        }
    }

    /**
     * check a between filter receives a correct size range
     * @param size1 in range
     * @param size2 in range
     * @return if valid
     */
    private static boolean invalidRange (double size1, double size2) {
        return size1 > size2;
    }

    /**
     * checks the condition to be tested for string containment filters
     * @param s line to be parsed
     * @return correct condition
     * @throws TypeOneException
     */
    private static boolean getCondition(String s) throws TypeOneException {
        checkSize(3);
        if (!yesOrNo(s)) {
            throw new TypeOneException();
        }
        return assignYesOrNo(s);
    }

    /**
     * check if value for a string based filter is correct
     * @param s line to be parsed
     * @return if correct
     */

    private static boolean yesOrNo(String s) {
        return (s.equals(YES) || s.equals(NO));
    }

    /**
     * assign correct value to the condition of a string based filter
     * @param s line to be parsed
     * @return correct condition
     */
    private static boolean assignYesOrNo(String s) {
        return s.equals(YES);
    }

    /**
     * return the correct string to be checked for string based filters
     * @return the string
     * @throws TypeOneException
     */
    private static String getStringInName() throws TypeOneException {
        checkSize(3);
        return filterParse[1];
    }

}

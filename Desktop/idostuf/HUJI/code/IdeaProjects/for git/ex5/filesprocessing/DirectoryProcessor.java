package filesprocessing;
import filesOrdering.*;
import filesFiltering.*;

import java.io.File;
import java.io.FileFilter;
import java.io.FileNotFoundException;
import java.util.Scanner;

//main class for file processing within a directory
public class DirectoryProcessor {

    private static FileFilter sectionFilter;
    private static ByOrder sectionOrder;
    private static int lineNum;
    private static int sectionLineNum;
    private static File commandsFile;
    private static File sourceDir;
    private static Scanner s;
    private static final String ARG_NUM_ERROR = "program takes precisely two arguments: source directory " +
            "and commands file", DIRECTORY_ERROR = "directory doesn't exist",
            COMMANDS_FILE_ERROR = "commands file doesn't exist", FILTER_ERROR = "FILTER subsection missing",
            ORDER_ERROR = "ORDER subsection missing", FILTER = "FILTER", ORDER = "ORDER", EMPTY = "";

    /**
     * main function: receives two arguments from user specifying a source directory and commands file by which to
     * filter and sort files within the directory
     */
    public static void main(String[] args) throws TypeTwoException {
        lineNum = 0; sectionLineNum = 0;
        try {
            argNumCheck(args);
            validDirectoryCheck(args);
            ParseCommandsFile(args);
        } catch (TypeTwoException e) {
            typeTwoError(e);
        }
    }

    /**
     * method that checks user inputted exactly two args
     * @param args inputs from user to run program
     */
    private static void argNumCheck(String[] args) {
        if (args.length != 2) {
            throw new TypeTwoException(ARG_NUM_ERROR);
        }
    }

    /**
     * checks that directory file has a valid path
     * @param args inputs from user
     */
    private static void validDirectoryCheck(String[] args) {
        sourceDir = new File(args[0]);
        if (!sourceDir.exists()) {
            throw new TypeTwoException(DIRECTORY_ERROR);
        }
    }

    /**
     * checks that commands file has a valid path, scans through commands file
     * @param args
     */
    private static void ParseCommandsFile(String[] args) {
        commandsFile = new File(args[1]);
        s = validCommandsFileCheck();
        while (s.hasNextLine()) {
            lineNum++; sectionLineNum++;
            String line = s.nextLine();
            parseLine(line);
        }
        completeSection();
    }

    /**
     * creates a new scanner object, if commands file path isn't valid throws exception
     * @return new scanner object
     */
    private static Scanner validCommandsFileCheck() {
        try {
            return new Scanner(commandsFile);
        } catch (FileNotFoundException e) {
            throw new TypeTwoException(COMMANDS_FILE_ERROR);
        }
    }

    /**
     * method for parsing each line in the commands file. Takes sections into consideration- checking if each
     * line in a section has the appropriate text
     * @param line current line to be parsed
     */
    private static void parseLine(String line) {
        switch (sectionLineNum % 4) {
            case 1:
                filterSubsectionCheck(line);
                break;
            case 2:
                makeSectionFilter(line);
                break;
            case 3:
                orderSubsectionCheck(line);
                break;
            case 0:
                makeSectionOrder(line);
                filterAndSort();
        }
    }

    /**
     * checks if every first line in new section holds the word "FILTER", if not throws exception
     * @param line to be read
     */
    private static void filterSubsectionCheck(String line) {
        if (!line.equals(FILTER)) {
            throw new TypeTwoException(FILTER_ERROR);
        }
    }

    /**
     * takes in second line of section, attempting to create the specified filter, or the default Allfilter
     * @param line to be read
     */
    private static void makeSectionFilter(String line) {
        try {
            sectionFilter = FileFilterFactory.getFileFilter(line);
        } catch (TypeOneException e) {
            typeOneError(e);
            //default filter
            sectionFilter = new AllFileFilter();
        }
    }

    /**
     * print warning and appropriate line to user
     * @param e exception thrown
     */
    private static void typeOneError(TypeOneException e) {
        System.err.println(e.getMessage() + lineNum);
    }

    /**
     * check if third line in a section holds the word "ORDER", otherwise throws an exception
     * @param line to be read
     */
    private static void orderSubsectionCheck(String line) {
        if (!line.equals(ORDER)) {
            throw new TypeTwoException(ORDER_ERROR);
        }
    }

    /**
     * takes in last line of section, attempting to create the specified order, or the default
     * ByAbsolutePath order.
     * @param line to be read
     */
    private static void makeSectionOrder(String line) {
        //if order subsection type is empty we skip to the second line of the next section
        if (line.equals(FILTER)) {
            sectionLineNum++;
            sectionOrder = new ByAbsolutePath();
        }  else {
            try {
                sectionOrder = OrderFactory.getOrder(line);
            } catch (TypeOneException e) {
                typeOneError(e);
                sectionOrder = new ByAbsolutePath();
            }
        }
    }

    /**
     * method which filters all files in the source directory by the specified order, and prints them.
     */
    private static void filterAndSort() {
        File[] filesFiltered = sourceDir.listFiles(sectionFilter);
        sectionOrder.sort(filesFiltered);
        printFiles(filesFiltered);
    }

    /**
     * method which prints all files in a sorted array.
     * @param filesFiltered array of files to print
     */
    private static void printFiles(File[] filesFiltered) {
        for (File f: filesFiltered) {
            if (f.isFile()) {
                System.out.println(f.getName());
            }
        }
    }

    /**
     * this method ensures we throw the correct error in case the file ends in mid-section.
     */
    private static void completeSection() {
        int sectionEnd = sectionLineNum % 4;
        if (sectionEnd == 1 || sectionEnd == 2) {
            //we're missing an order subsection
                throw new TypeTwoException(ORDER_ERROR);
        } else if (sectionEnd == 3) {
            //order type is empty, so we sort by absolute path
            sectionOrder = new ByAbsolutePath();
            filterAndSort();
        }
    }

    /**
     * this method ensures we print the correct error in case of a TypeTwoException, and ends the program.
     * @param e typeTwoException
     */
    private static void typeTwoError(TypeTwoException e) {
        System.err.println(e.getMessage());
        System.exit(-1);
    }
}



package filesOrdering;

import filesprocessing.TypeOneException;

/**
 * this class is in charge of creating a new order object and throwing exceptions in case of invalid input
 */
public class OrderFactory {

    private static final String TOKEN = "#";
    private static final String REVERSE = "REVERSE";
    private static final String ABS = "abs";
    private static final String SIZE = "size";
    private static final String TYPE = "type";

    /**
     * this function attempts to create a new order according to input
     * @param toParse the string we are parsing
     * @return a new ByOrder object
     * @throws TypeOneException type one error
     */
    public static ByOrder getOrder(String toParse) throws TypeOneException {
        String[] orderParse = toParse.split(TOKEN);
        boolean isReversed = reverseCheck(orderParse);
        String orderType = orderParse[0];
        switch (orderType) {
                case ABS:
                    return new ByAbsolutePath(isReversed);
                case SIZE:
                    return new BySize(isReversed);
                case TYPE:
                    return new ByFileType(isReversed);
                default :
                    //illegal order values (excluding FILTER, see DirectoryProcessor lines 162-164)
                    throw new TypeOneException();
        }

        }

    /**
     * this method checks if a reverse argument was given for the order type
     * @param orderParse string to be parsed
     * @return true if order is reversed
     */
    private static boolean reverseCheck(String[] orderParse) {
        if (orderParse.length > 1) {
            //an order object may have only one reverse argument at its end
            String reverse = orderParse[1];
            return reverse.equals(REVERSE);
        }
        return false;
    }
}


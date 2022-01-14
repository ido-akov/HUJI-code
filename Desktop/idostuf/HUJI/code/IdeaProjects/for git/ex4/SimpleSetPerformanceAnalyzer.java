import java.util.*;

public class SimpleSetPerformanceAnalyzer {

    private static final String[] DATA1 = Ex4Utils.file2array("data1.txt");
    private static final String[] DATA2 = Ex4Utils.file2array("data2.txt");

    private static SimpleSet OpenHash;
    private static SimpleSet ClosedHash;
    private static SimpleSet Tree;
    private static SimpleSet Linked;
    private static SimpleSet JavaHash;
    private static SimpleSet[] DATA_STRUCTURES;
    private static HashMap<Integer,String> STRUCTURE_NAMES = new HashMap<>();

    private final static String MILLISECONDS = " milliseconds";
    private final static String NANOSECONDS = " nanoseconds";
    private final static String OPERATION_MESSAGE = " to complete operation for set";


    private long timeBefore;
    private long difference;
    private static final int ITERATIONS = 70000;
    private static final int LINKED_ITERATIONS = 7000;

    private void initializeStructures() {
        OpenHash = new OpenHashSet();
        ClosedHash = new ClosedHashSet();
        Tree = new CollectionFacadeSet(new TreeSet<>());
        Linked = new CollectionFacadeSet(new LinkedList<>());
        JavaHash = new CollectionFacadeSet(new HashSet<>());
        DATA_STRUCTURES = new SimpleSet[]{OpenHash, ClosedHash, Tree, Linked, JavaHash};
    }
    private void initializeStructureNames() {
        STRUCTURE_NAMES.put(OpenHash.hashCode(), " OpenHash");
        STRUCTURE_NAMES.put(ClosedHash.hashCode(), " ClosedHash");
        STRUCTURE_NAMES.put(Tree.hashCode(), " Treeset");
        STRUCTURE_NAMES.put(Linked.hashCode(), " LinkedList");
        STRUCTURE_NAMES.put(JavaHash.hashCode(), " HashSet(java)");
    }

    private long milliSeconds (long nano) {
        return nano / 1000000;
    }

    private long approximation (int loops, long nano) {
        return nano / loops;
    }


    public void addData (String[] data) {
        System.out.println("add new data:");
        initializeStructures();
        initializeStructureNames();
        for (SimpleSet set: DATA_STRUCTURES) {
            timeBefore = System.nanoTime();
            for (String s: data) {
                set.add(s);
            }
            difference = System.nanoTime() - timeBefore;
            System.out.println(milliSeconds(difference) + MILLISECONDS + OPERATION_MESSAGE +
                STRUCTURE_NAMES.get(set.hashCode()));
        }
    }

    public void containsString (String Val) {
        System.out.println("contains " + Val + ":");
        for (SimpleSet set: DATA_STRUCTURES) {
            int iters = ITERATIONS;
            if (STRUCTURE_NAMES.get(set.hashCode()).equals("LinkedList")) {
                iters = LINKED_ITERATIONS;
            } else {
                ContainsLoop(Val, set, iters);
            }
            timeBefore = System.nanoTime();
            ContainsLoop(Val, set, iters);
            difference = System.nanoTime() - timeBefore;
            System.out.println(approximation(iters,difference) + NANOSECONDS + OPERATION_MESSAGE +
                STRUCTURE_NAMES.get(set.hashCode()));
        }
    }

    private void ContainsLoop(String Val, SimpleSet set, int iters) {
        for (int i = 0; i < iters; i++ ) {
            set.contains(Val);
        }
    }

    public static void main(String[] args) {
        SimpleSetPerformanceAnalyzer analyze = new SimpleSetPerformanceAnalyzer();
        analyze.addData(DATA1);
        analyze.containsString("hi");
        analyze.containsString("-13170890158");
        analyze.addData(DATA2);
        analyze.containsString("23");
        analyze.containsString("hi");
    }

}

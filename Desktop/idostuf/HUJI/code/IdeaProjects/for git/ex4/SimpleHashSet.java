public abstract class SimpleHashSet implements SimpleSet {

    protected static final float DEFAULT_HIGHER_CAPACITY = (float) 3 / 4;
    protected static final float DEFAULT_LOWER_CAPACITY = (float) 1 / 4;
    protected static final int INITIAL_CAPACITY = 16;
    protected static final float INCREASE = 2;
    protected static final float DECREASE = (float) 1 / 2;

    protected float upperLoadFactor;
    protected float lowerLoadFactor;
    protected int tableSize = INITIAL_CAPACITY;
    protected int numOfStringsInTable;


    //constructors
    protected SimpleHashSet() {
        setLoadFactors(DEFAULT_HIGHER_CAPACITY, DEFAULT_LOWER_CAPACITY);
    }

    protected SimpleHashSet(float upperLoadFactor,
                            float lowerLoadFactor) {
        setLoadFactors(upperLoadFactor, lowerLoadFactor);
    }

    protected SimpleHashSet(java.lang.String[] data) {
        this();
        for (String d : data) {
            add(d);
        }
    }

    private void setLoadFactors(float upperLoadFactor, float lowerLoadFactor) {
        this.upperLoadFactor = upperLoadFactor;
        this.lowerLoadFactor = lowerLoadFactor;
    }

    public int capacity() {
        return tableSize;
    }

    protected int clamp(int index) {
        //the index is different according to open or closed hashing mechanism
        return index & (capacity() - 1);
    }

    protected float getLowerLoadFactor() {
        return this.lowerLoadFactor;
    }

    protected float getUpperLoadFactor() {
        return this.upperLoadFactor;
    }

    /**
     * Add a specified element to the set if it's not already in it.
     *
     * @param newValue New value to add to the set
     * @return False iff newValue already exists in the set
     */
    public abstract boolean add(String newValue);

    /**
     * Look for a specified value in the set.
     *
     * @param searchVal Value to search for
     * @return True iff searchVal is found in the set
     */
    public abstract boolean contains(String searchVal);

    /**
     * Remove the input element from the set.
     *
     * @param toDelete Value to delete
     * @return True iff toDelete is found and deleted
     */
    public abstract boolean delete(String toDelete);

    /**
     * @return The number of elements currently in the set
     */
    public abstract int size();

    protected void resize(float sizeChange) {
        tableSize = (int) (sizeChange * capacity());
    }



//    protected boolean canBeAdded(String newValue) {
//        if (contains(newValue)) {
//            return false;
//        }
}



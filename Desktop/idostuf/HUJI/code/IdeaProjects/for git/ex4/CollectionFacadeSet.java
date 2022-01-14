public class CollectionFacadeSet implements SimpleSet {

    protected java.util.Collection<java.lang.String> collection;

    // constructor
    public CollectionFacadeSet(java.util.Collection<java.lang.String> collection) {
        this.collection = collection;
    }
    /**
     * Add a specified element to the set if it's not already in it.
     * @param newValue New value to add to the set
     * @return False iff newValue already exists in the set
     */
    public boolean add(java.lang.String newValue) {
        if (collection.contains(newValue)) {
            //LinkedList is not a set naturally, so we need to constraint it to hold only one of each given value
            return false;
        }
        return collection.add(newValue);
    }

    /**
     * Look for a specified value in the set.
     * @param searchVal Value to search for
     * @return True iff searchVal is found in the set
     */
    public boolean contains(String searchVal) {
        return collection.contains(searchVal);
    };
    /**
     * Remove the input element from the set.
     * @param toDelete Value to delete
     * @return True iff toDelete is found and deleted
     */
    public boolean delete(String toDelete) {
        return collection.remove(toDelete);
    };
    /**
     * @return The number of elements currently in the set
     */
    public int size() {
        return collection.size();
    }
}

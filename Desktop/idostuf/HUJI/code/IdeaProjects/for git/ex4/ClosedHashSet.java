public class ClosedHashSet extends SimpleHashSet {

    private static final String DELETED = new String("");
    private static final int ERROR_MESSAGE = -1;


    private String[] hashTable = new String[tableSize];

    public ClosedHashSet() {
        super();
    }

    public ClosedHashSet(float upperLoadFactor, float lowerLoadFactor) {
        super(upperLoadFactor, lowerLoadFactor);
    }

    public ClosedHashSet(java.lang.String[] data) {
        this();
        for (String s: data){
            add(s);
        }
    }

    public int capacity() {
        return super.capacity();
    }

    public int size() {
        return numOfStringsInTable;
    }

    @Override
    public boolean add(String newValue) {
        if (newValue == null || contains(newValue)) {
            return false;
        } else {
            int index = getEmptyCell(hashTable, newValue);
            if (index == ERROR_MESSAGE) {
                //table is full
                return false;
            }
            hashTable[index] = newValue;
            numOfStringsInTable++;
            if ((float) size() / capacity() > upperLoadFactor) {
                rehash(INCREASE);
            }
            return true;
        }
    }

    private int findCell(String[] table, String searchValue) {
        int hashIndex = searchValue.hashCode();
            //we are using this loop to find the index of strings we know are within the hash table.
            //Therefore we will always get the right index of the string.
            for (int i = 0; i < capacity(); i++) {
                int probe = getProbe(i);
                int currentIndex = clamp(hashIndex + probe);
                String currentString = hashTable[currentIndex];
                if (currentString == searchValue) {
                    return currentIndex;
                } else if (currentString == null) {
                    break;
                }
            }
        return ERROR_MESSAGE;
    }

    private int getEmptyCell(String[] table, String newValue) {
        int hashIndex = newValue.hashCode();
        for (int i = 0; i < capacity(); i++) {
            int probe = getProbe(i);
            int currentIndex = clamp(hashIndex + probe);
            String currentString = table[currentIndex];
            if (currentString == null) {
                return currentIndex;
            }
        }
        return ERROR_MESSAGE;
    }
        
        
    private int getProbe(int i) {
        return (i + i * i) / 2;
    }

    
    private void rehash(float sizeChange) {
        super.resize(sizeChange);
        String[] newHashTable = new String[tableSize];
        for (String s : hashTable) {
            if (s != null && s != DELETED) {
                int index = getEmptyCell(newHashTable, s);
                newHashTable[index] = s;
            }
        }
        hashTable = newHashTable;
    }

    @Override
    public boolean contains(String searchVal) {
        int index = findCell(hashTable, searchVal);
        if (index == ERROR_MESSAGE) {
            return false;
        } else {
            return true;
        }
    }

    @Override
    public boolean delete(String toDelete) {
        if (toDelete == null || !contains(toDelete)) {
            return false;
        } else {
            int index = findCell(hashTable, toDelete);
            hashTable[index] = DELETED;
            numOfStringsInTable--;
            if ((float) size() / capacity() < lowerLoadFactor) {
                rehash(DECREASE);
            }
            return true;
        }
    }
}
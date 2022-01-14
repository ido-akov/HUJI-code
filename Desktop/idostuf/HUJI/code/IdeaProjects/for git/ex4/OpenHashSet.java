import java.util.LinkedList;

public class OpenHashSet extends SimpleHashSet {


    private LinkedFacade[] hashTable = new LinkedFacade[tableSize];


    //constructors
    public OpenHashSet() {
        super();
        fillTable(hashTable);
    }

    public OpenHashSet(float upperLoadFactor, float lowerLoadFactor) {
        super(upperLoadFactor, lowerLoadFactor);
        fillTable(hashTable);
    }

    public OpenHashSet(java.lang.String[] data) {
        this();
        for (String d : data) {
            add(d);
        }
    }

    private void fillTable(LinkedFacade[] table) {
        for (int i = 0; i < tableSize; i++) {
            table[i] = new LinkedFacade();
        }
    }

    public int capacity() {
        return super.capacity();
    }

    @Override
    public boolean add(String newValue) {
        if (newValue == null || contains(newValue)) {
            return false;
        } else {
            addStringToTable(newValue, hashTable);
            numOfStringsInTable++;
            if ((float) size() / capacity() > upperLoadFactor) {
                rehash(INCREASE);
            }
            return true;
        }
    }

    private void addStringToTable(String newValue, LinkedFacade[] table) {
        int index = getHashIndex(newValue);
        table[index].add(newValue);
    }

    private int getHashIndex(String newValue) {
        return clamp(newValue.hashCode());
    }

    protected int clamp(int index) {
        return super.clamp(index);
    }

    private void rehash(float sizeChange) {
        super.resize(sizeChange);
        LinkedFacade[] newHashTable = new LinkedFacade[tableSize];
        fillTable(newHashTable);
        for (LinkedFacade l : hashTable) {
            if (l.size() > 0) {
                for (String s: l) {
                    addStringToTable(s, newHashTable);
                }
            }
        }
        hashTable = newHashTable;
    }

    public int size() {
        return numOfStringsInTable;
    }

    @Override
    public boolean contains(String searchVal) {
        int index = getHashIndex(searchVal);
        return hashTable[index].contains(searchVal);
    }

    @Override
    public boolean delete(String toDelete) {
        if (contains(toDelete)) {
            int index = getHashIndex(toDelete);
            hashTable[index].delete(toDelete);
            numOfStringsInTable--;
            if ((float) size() / capacity() < lowerLoadFactor) {
                rehash(DECREASE);
            }
            return true;
        } else {
            return false;
        }
    }

}


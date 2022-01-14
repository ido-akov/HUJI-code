import java.util.Iterator;
import java.util.LinkedList;

public class LinkedFacade extends CollectionFacadeSet implements Iterable<String> {


    public LinkedFacade() {
        super(new LinkedList<String>());
    }

    @Override
    public boolean add(String newValue) {
        return super.add(newValue);
    }

    @Override
    public boolean contains(String searchVal) {
        return super.contains(searchVal);
    }

    @Override
    public boolean delete(String toDelete) {
        return super.delete(toDelete);
    }

    @Override
    public Iterator<String> iterator() {
        return collection.iterator();
    }
}

package common;

import java.util.ArrayList;

/**
 * Created by vld on 1/22/17.
 */
public class StringPool {
    private ArrayList<String> pool;

    public StringPool() { pool = new ArrayList<>(); }

    public void pushString(String str) {
        pool.add(str);
    }

    public int indexOf(String str) {
        return (int)pool.indexOf(str);
    }

    public long sizeInBytes() {
        long size = 0;
        for( String str : pool ) size += str.length() + 1;

        return size;
    }

    public ArrayList<String> getContainer() { return pool; }
}

package readers;

import common.StringPool;

import java.util.Scanner;

/**
 * Created by vld on 1/22/17.
 */

public class StringPoolReader {
    private StringPool pool;

    public StringPoolReader() {
        pool = new StringPool();
    }

    /**
     * @param scanner
     * @throws Exception
     *
     * This method declared for reading strings by format "string"
     */
    public void readStringPool(Scanner scanner) throws Exception {

        while( scanner.hasNext("\"[^ \"]*\"")) {
            String str = scanner.next();
            pool.pushString(str.substring(1, str.length() - 1));
        }
    }

    public StringPool getStringPool() {
        return pool;
    }
}

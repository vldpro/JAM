package readers;

import common.MnemonicsList;
import common.Function;
import common.StringPool;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;

/**
 * Created by vld on 1/22/17.
 * This class define
 * @author vld
 */
public class FunctionReader {
    private ArrayList<Function> functions;

    public FunctionReader() {
        functions = new ArrayList<>();
    }

    /**
     * @param pool - string pool for compare method name with constant pool strings
     * @param scanner - for reading
     * @throws Exception
     */

    public void readFunction(StringPool pool, Scanner scanner) throws Exception {
        Function newFunction = new Function();

        readDefinition( pool, newFunction, scanner );
        readCommands( newFunction, scanner );

        functions.add(newFunction);
    }

    public Function[] getReadedFunction() {
        Function[] asFunctionsArray = new Function[functions.size()];
        int i = 0;
        for( Function meta : functions ) asFunctionsArray[i++] = meta;

        return asFunctionsArray;
    }

    private void readDefinition(
            StringPool pool,
            Function newFunction,
            Scanner scanner) throws Exception
    {
        if( scanner.hasNext() ) {
            newFunction.setNameID( pool.indexOf(scanner.next()) );
        } else throw new Exception("Can't read function name. ");

        scanner.skip("[ \t\n]*");

        if( scanner.hasNext("args") ) {
            scanner.skip("[ ]*args[ ]*");

            if( scanner.hasNextLong() )
                newFunction.setArgsCount(scanner.nextLong());

        } else throw new Exception(" \"args\" keyword not found. ");
    }

    private void readCommands(Function newFunction, Scanner scanner) throws Exception {
        int currentPos = 0;
        HashMap<String, Integer> labels = new HashMap<>();

        while(scanner.hasNext()) {
            String cmdMnemonic = scanner.next();

            if( cmdMnemonic.matches("[^:]*:")) {
                labels.put( cmdMnemonic.substring(0, cmdMnemonic.length() - 1), currentPos );

            } else if( cmdMnemonic.equals("push") ) {
                newFunction.pushBytecode(MnemonicsList.getBytecode(cmdMnemonic));

                if( scanner.hasNextLong() ) {
                    newFunction.pushConstant(scanner.nextLong());

                } else if( scanner.hasNextDouble() ) {
                    newFunction.pushConstant(scanner.nextDouble());

                } else if( scanner.hasNext() ) {
                    newFunction.pushConstant( labels.get(scanner.next()) );

                } else throw new Exception("after \"push\" command no argumets.");

                currentPos += 8;

            } else  if( cmdMnemonic.equals("end") ) {
                break;

            } else newFunction.pushBytecode( MnemonicsList.getBytecode(cmdMnemonic) );

            currentPos++;
        }
    }


}

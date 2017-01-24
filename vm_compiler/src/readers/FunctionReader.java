package readers;

import common.MnemonicsList;
import common.Function;
import common.StringPool;

import java.math.BigInteger;
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
        for( Function function : functions ) asFunctionsArray[i++] = function;

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

        if( scanner.hasNext("argc") ) {
            scanner.skip("[ ]*argc[ ]*");

            if( scanner.hasNextLong() )
                newFunction.setArgsCount(scanner.nextLong());

        } else throw new Exception(" \"argc\" keyword not found. ");
    }

    private void readCommands(Function newFunction, Scanner scanner) throws Exception {
        int currentPos = 0;
        HashMap<String, Integer> labels = new HashMap<>();
        HashMap<String, Integer> unresolvedLabels = new HashMap<>();

        while( scanner.hasNext() ) {
            String cmdMnemonic = scanner.next();

            if( cmdMnemonic.matches("[^:]*:")) {
                String label = cmdMnemonic.substring(0, cmdMnemonic.length() - 1 );

                if( unresolvedLabels.get(label) != null ) {
                    newFunction.setConstant( currentPos, unresolvedLabels.get(label) );

                } else labels.put( cmdMnemonic.substring(0, cmdMnemonic.length() - 1), currentPos );

            } else if( cmdMnemonic.equals("push") ) {
                long ptrToPush = newFunction.pushBytecode( MnemonicsList.getBytecode(cmdMnemonic) );

                if ( scanner.hasNextLong() ) {
                    newFunction.pushConstant(scanner.nextLong());

                } else if( scanner.hasNextDouble() ) {
                    newFunction.pushConstant( scanner.nextDouble() );

                } else if( scanner.hasNext("0x[0-9A-Fa-f]+") ) {
                    long val = new BigInteger(scanner.next().substring(2), 16).longValue();
                    newFunction.pushConstant(val);

                } else if( scanner.hasNext("'[^\"]'") ) {
                    byte asAscii = (byte)scanner.next().charAt(1);;
                    newFunction.pushConstant(asAscii);

                } else if( scanner.hasNext() ) {
                    String label = scanner.next();
                    Integer isResolvedLabel = labels.get(label);

                    if( isResolvedLabel != null ) newFunction.pushConstant( labels.get(scanner.next()) );
                    else {
                        newFunction.pushConstant( 0 );
                        unresolvedLabels.put(label, (int)ptrToPush);
                    }

                } else throw new Exception("after \"push\" command no argumets.");

                currentPos += 8;

            } else  if( cmdMnemonic.equals("end") ) {
                break;

            } else if( ! MnemonicsList.isCorrectMnemonic(cmdMnemonic) ) {
                throw new Exception();

            } else newFunction.pushBytecode( MnemonicsList.getBytecode(cmdMnemonic) );

            currentPos++;
        }
    }
}

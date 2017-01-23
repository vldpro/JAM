import common.Function;
import common.StringPool;
import readers.FunctionReader;
import readers.StringPoolReader;
import writer.BinaryFileOutput;

import java.io.DataOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Scanner;
import java.util.stream.Stream;

import static java.nio.file.StandardOpenOption.CREATE;
import static java.nio.file.StandardOpenOption.WRITE;

/**
 * Created by vld on 1/22/17.
 */
public class Main {
    public static void main(String[] args) {
        if( args.length == 0) {
            System.err.println("No input files. Syntax: vmcompiler source dest");
            System.exit(1);
        } else if ( args.length == 1 ) {
            System.err.println("Not specified file for compiled code. ");
        }

        File source = new File(args[0]);
        StringPoolReader poolReader = new StringPoolReader();
        FunctionReader functionReader = new FunctionReader();

        try (Scanner scanner = new Scanner(source)) {
            while(scanner.hasNext()) {
                if (scanner.hasNext("[ ]*def[ ]*")) {
                    scanner.skip("def");

                    if( scanner.hasNext("constpool")) {
                        scanner.skip("[ ]*constpool[ ]*");

                        poolReader.readStringPool(scanner);
                    } else if( scanner.hasNext("function")) {
                        scanner.skip("[ ]*function[ ]*");

                        functionReader.readFunction(poolReader.getStringPool(), scanner);
                    }

                    scanner.skip("[ \t\n]*");
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace(); //TODO

        } catch (Exception e) {
            e.printStackTrace(); //TODO
        }

        printResultOfReading(functionReader.getReadedFunction(), poolReader.getStringPool());

        try {
            writeFile( args[1], functionReader.getReadedFunction(), poolReader.getStringPool() );

        } catch ( IOException  e ) {
            e.printStackTrace();
        }
    }

    public static void printResultOfReading( Function[] funcs, StringPool pool ) {
        System.out.println("STRING POOL");
        pool.getContainer().forEach(System.out::println);
        System.out.println("\nFUNCTIONS");
        Stream.of(funcs).forEach(Function::printFunctionInfo);
    }

    public static void writeFile(String filename, Function[] functions, StringPool stringPool) throws IOException {
        Path filePath = Paths.get(filename);

        try(DataOutputStream out = new DataOutputStream(Files.newOutputStream(filePath, CREATE, WRITE)) ) {
            BinaryFileOutput binaryFileOutput = new BinaryFileOutput(out);

            binaryFileOutput.writeFileHeader(
                   0xDEAD_DEAD_DEAD_DEADL, 1, stringPool.sizeInBytes(), functions.length
            );

            for( String str: stringPool.getContainer() )
                binaryFileOutput.writeASCIIString(str);

            for( Function function : functions )
                binaryFileOutput.writeFunctionMeta(function);

        }
    }
}

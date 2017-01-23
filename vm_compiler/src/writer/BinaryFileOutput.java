package writer;

import common.Endianness;
import common.Function;

import java.io.DataOutputStream;
import java.io.IOException;

/**
 * Created by vld on 1/23/17.
 */
public class BinaryFileOutput {
    private DataOutputStream out;

    public BinaryFileOutput(DataOutputStream out) {
        this.out = out;
    }

    public void writeFileHeader( long tag, long version, long stringPoolSize, long functionsCount )
            throws IOException {

        out.write( Endianness.getLongAsByteArray(tag) );
        out.write( Endianness.getLongAsByteArray(version) );
        out.write( Endianness.getLongAsByteArray(stringPoolSize) );
        out.write( Endianness.getLongAsByteArray(functionsCount) );
    }

    public void writeASCIIString( String str ) throws IOException {
        out.write( Endianness.getStringAsByteArray(str) );
    }

    public void writeFunctionMeta( Function function ) throws IOException {
        out.write( Endianness.getLongAsByteArray(function.getNameID()) );
        out.write( Endianness.getLongAsByteArray(function.getArgsCount()) );
        out.write( Endianness.getLongAsByteArray(function.getCommandsCount()) );
        out.write( function.getBytecodesAsByteArray() );
    }

}

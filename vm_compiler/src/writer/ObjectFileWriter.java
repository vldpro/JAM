package writer;

import common.Endianness;
import common.Function;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by vld on 1/23/17.
 * Byte Order: Little Endian
 * Code Point: ASCII
 */
public class ObjectFileWriter extends DataOutputStream {

    public ObjectFileWriter(OutputStream out) {
        super(out);
    }

    public void writeFileHeader( long tag, long version, long stringPoolSize, long functionsCount )
            throws IOException {

        super.write( Endianness.getLongAsByteArray(tag) );
        super.write( Endianness.getLongAsByteArray(version) );
        super.write( Endianness.getLongAsByteArray(stringPoolSize) );
        super.write( Endianness.getLongAsByteArray(functionsCount) );
    }

    public void writeASCIIString( String str ) throws IOException {
        super.write( Endianness.getStringAsByteArray(str) );
    }

    public void writeFunction(Function function ) throws IOException {
        super.write( Endianness.getLongAsByteArray(function.getNameID()) );
        super.write( Endianness.getLongAsByteArray(function.getArgsCount()) );
        super.write( Endianness.getLongAsByteArray(function.getCommandsCount()) );
        super.write( function.getBytecodesAsByteArray() );
    }
}

package common;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;

/**
 * Created by vld on 1/23/17.
 */
public class Endianness {
    private Endianness() {}

    /**
     * @param value
     * @return long value as bytes in Little Endian order by default
     */
    public static byte[] getLongAsByteArray( long value ) {
        return getLongAsByteArray( value, ByteOrder.LITTLE_ENDIAN );
    }

    /**
     * @param value
     * @param order is order of bytes in return value
     * @return
     */

    public static byte[] getLongAsByteArray( long value, ByteOrder order ) {
        ByteBuffer asByteBuffer = ByteBuffer.allocate( Long.SIZE / Byte.SIZE );

        asByteBuffer.order( order );
        asByteBuffer.putLong( value );

        return asByteBuffer.array();
    }



    public static byte[] getDoubleAsByteArray( double value ) {
        return getDoubleAsByteArray( value, ByteOrder.LITTLE_ENDIAN );
    }

    public static byte[] getDoubleAsByteArray( double value, ByteOrder order ) {
        ByteBuffer asByteBuffer = ByteBuffer.allocate( Double.SIZE / Byte.SIZE );

        asByteBuffer.order( order );
        asByteBuffer.putDouble(value);

        return asByteBuffer.array();
    }


    /**
     *
     * @param str source string
     * @return byte[] - array of bytes in Little Endian order by default
     */
    public static byte[] getStringAsByteArray( String str ) {
        return getStringAsByteArray( str, ByteOrder.LITTLE_ENDIAN );
    }

    public static byte[] getStringAsByteArray( String str, ByteOrder order ) {
        ByteBuffer asByteBuffer = ByteBuffer.allocate( str.length() + 1 );

        asByteBuffer.order( order );
        asByteBuffer.put( str.getBytes(StandardCharsets.US_ASCII) );

        return asByteBuffer.array();
    }
}

package common;

import java.util.ArrayList;

/**
 * Created by vld on 1/22/17.
 */
public class Function {
    private long nameID;
    private long argsCount;
    private long commandsCount;

    private ArrayList<Byte> bytecodes;

    public Function() {
        bytecodes = new ArrayList<>();
    }

    public Function(long nameID, long argsCount ) {
        this.nameID = nameID; this.argsCount = argsCount;
        bytecodes = new ArrayList<>();
    }

    public void setArgsCount( long argsCount ) {
        this.argsCount = argsCount;
    }

    public long getArgsCount() {
        return argsCount;
    }

    public void setNameID( long nameID ) {
        this.nameID = nameID;
    }

    public long getNameID() {
        return nameID;
    }

    public void printFunctionInfo() {

        System.out.println("name ID: " + nameID);
        System.out.println("argsCount: " + argsCount);
        System.out.println("commandsCount: " + commandsCount);

        for( int i = 0; i < commandsCount; i++ ) {
            System.out.print("\t" + MnemonicsList.getMnemonic(bytecodes.get(i)) );

            if( bytecodes.get(i) == 0x1 ) {
                long constAsLong = 0;

                for(int j = i + 1, k = 0; j < i + 9; j ++, k++ )
                    //asBigEndian: constAsLong = (constAsLong << 8) + (bytecodes.get(j) & 0xff);
                    constAsLong += ((long) bytecodes.get(j) & 0xffL) << (8 * k); //asLittleEndian


                System.out.print(" " + constAsLong );

                i += 8;
            }

            System.out.print("\n");
        }
    }

    public long getCommandsCount() { return commandsCount; }

    public byte[] getBytecodesAsByteArray() {
        byte[] asByteArray = new byte[ bytecodes.size() ];
        int i = 0;

        for( Byte bt : bytecodes ) asByteArray[i++] = bt;

        return asByteArray;
    }

    public long pushBytecode( byte bytecode ) {
        bytecodes.add( bytecode );
        commandsCount++;

        return commandsCount;
    }

    public void setConstant(long constant, int offset ) {
        byte[] constantByBytes = Endianness.getLongAsByteArray(constant);

        int i = offset;
        for( byte bt : constantByBytes ) bytecodes.set(i++, bt);

    }

    public void pushConstant( long constant ) {
        byte[] constantByBytes = Endianness.getLongAsByteArray(constant);

        for(byte bt : constantByBytes) bytecodes.add(bt);
        commandsCount += 8;
    }

    public void pushConstant( double constant ) {
        byte[] constantByBytes = Endianness.getDoubleAsByteArray(constant);

        for(byte bt : constantByBytes) bytecodes.add(bt);
        commandsCount +=8;
    }

}

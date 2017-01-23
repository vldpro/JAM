package common;

import java.util.ArrayList;

/**
 * Created by vld on 1/22/17.
 */
public class MnemonicsList {
    private static ArrayList<String> mnemonics = new ArrayList<>(256);

    private MnemonicsList() {}

    static {
        mnemonics = new ArrayList<>(256);

        mnemonics.add("nop"); mnemonics.add("push");
        mnemonics.add("iadd"); mnemonics.add("dadd");
        mnemonics.add("isub"); mnemonics.add("dsub");
        mnemonics.add("idiv"); mnemonics.add("ddiv");
        mnemonics.add("imul"); mnemonics.add("dmul");
        mnemonics.add("mod");
        mnemonics.add("branch"); mnemonics.add("branchif");
        mnemonics.add("invoke");
        mnemonics.add("retvoid"); mnemonics.add("ret");
        mnemonics.add("dup"); mnemonics.add("swap");

        mnemonics.add("iprint"); mnemonics.add("dprint"); mnemonics.add("sprint");
        mnemonics.add("halt");

        mnemonics.add("icmpeq"); mnemonics.add("icmpne");
        mnemonics.add("icmpg");  mnemonics.add("icmpge");
        mnemonics.add("icmpl");  mnemonics.add("icmple");

        mnemonics.add("dcmpeq"); mnemonics.add("dcmpne");
        mnemonics.add("dcmpg");  mnemonics.add("dcmpge");
        mnemonics.add("dcmpl");  mnemonics.add("dcmple");

        mnemonics.add("neg");
        mnemonics.add("i2d"); mnemonics.add("d2i");
        mnemonics.add("load"); mnemonics.add("strore");

        mnemonics.add("dinc"); mnemonics.add("ddec");
        mnemonics.add("iinc"); mnemonics.add("idec");

    }

    public static byte getBytecode( String mnemonic ) {
        return (byte) mnemonics.indexOf(mnemonic);
    }

    public static String getMnemonic( byte bytecode ) {
        return mnemonics.get(bytecode);
    }
}

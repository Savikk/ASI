package des;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
/**
 * @author Savikk
 */

class pliki {

    public static ArrayList<String> readFile(String file) {
	ArrayList<String> fileContent = new ArrayList<String>();
	try {
	    BufferedReader in = new BufferedReader(new FileReader(file));
	    String strLine;
	    while ((strLine = in.readLine()) != null) {
		fileContent.add(strLine);
	    }
	    in.close();

	} catch (Exception e) {
	    System.err.println("Error: " + e.getMessage());
	}
	return fileContent;
    }

    public static void writeFile(String file, String content, Boolean append) {
	try {
	    BufferedWriter out = new BufferedWriter(new FileWriter(file, append));
	    out.write(content);
	    out.close();
	} catch (Exception e) {
	    System.err.println("Error: " + e.getMessage());
	}
    }

    public static void clearFile(String file) {
	writeFile(file, new String(), false);
    }
}
public class miniDES {
	
    
    private static final String PLAIN = "plain.txt";
    private static final String CRYPTO = "crypto.txt";
    private static final String DECRYPT = "decrypt.txt";
    private static final String PKEY= "key.txt";
    private static final String ANAL = "analyze.txt";

    private static final String PERMUTACJA = "12434356";
    private static String KEY;

    private static String[] S1 = {"101", "010", "001", "110", "011", "100", 
    "111", "000", "001", "100", "110", "010", "000", "111", "101", "011"};
    private static String[] S2 = {"100", "000", "110", "101", "111", "001",
    "011", "010", "101", "011", "000", "111", "110", "010", "001", "100"};
    
    
    public static void main(String[] args) {
	String optionFunction = args[0];
	KEY = pliki.readFile(PKEY).get(0);
	if (optionFunction.equals("-e")) {
	    System.out.println("Szyfrowanie zakonczone ");
	    pliki.clearFile(CRYPTO);
	    pliki.writeFile(CRYPTO, encrypt(pliki.readFile(PLAIN).get(0)), true);
	} else if (optionFunction.equals("-d")) {
	    System.out.println("Deszyfr Zakonczony ");
	    pliki.clearFile(DECRYPT);
	    pliki.writeFile(DECRYPT, decrypt(pliki.readFile(CRYPTO).get(0)), true);
	} else if (optionFunction.equals("-a")) {
	    System.out.println("Analiza Zakonczona ");
	    pliki.clearFile(ANAL);
	    pliki.writeFile(ANAL, analyze(pliki.readFile(PLAIN).get(0)), true);
	}
    }

    public static String analyze(String plain) {

	StringBuilder result = new StringBuilder();
	String[] inStrings = plain.split(" ");
	result.append(inStrings[0] + "\t" + inStrings[1] + "\t" + xorBits(inStrings[0], inStrings[1]) + "\n");

	String[] L1 = new String[9];
	String[] R1 = new String[9];
	L1[0] = inStrings[0].substring(0, inStrings[0].length()/2);
	R1[0] = inStrings[0].substring(inStrings[0].length()/2, inStrings[0].length());


	String[] L2 = new String[9];
	String[] R2 = new String[9];
	L2[0] = inStrings[1].substring(0, inStrings[1].length()/2);
	R2[0] = inStrings[1].substring(inStrings[1].length()/2, inStrings[1].length());

	for (int i = 1; i <= 8; i++) {
	    L1[i] = R1[i-1];
	    R1[i] = xorBits(L1[i-1], fFunction(xorBits(eFunction(R1[i-1]), rotKey(KEY, i))));

	    L2[i] = R2[i-1];
	    R2[i] = xorBits(L2[i-1], fFunction(xorBits(eFunction(R2[i-1]), rotKey(KEY, i))));

	    result.append(L1[i]+R1[i] + "\t" + L2[i]+R2[i] + "\t" + xorBits(L1[i]+R1[i], L2[i]+R2[i]) + "\n");
	}

	return result.toString();
    }

    public static String decrypt(String plain) {

	String[] L = new String[9];
	String[] R = new String[9];
	L[0] = plain.substring(0, plain.length()/2);
	R[0] = plain.substring(plain.length()/2, plain.length());

	for (int i = 1; i <= 8; i++) {
	    L[i] = R[i-1];
	    R[i] = xorBits(L[i-1], fFunction(xorBits(eFunction(R[i-1]), rotKey(KEY, 8-(i-1)))));
	}

	return R[8]+L[8];
    }

    public static String encrypt(String plain) {
	plain = plain.split(" ")[0];
	String[] L = new String[9];
	String[] R = new String[9];
	L[0] = plain.substring(0, plain.length()/2);
	R[0] = plain.substring(plain.length()/2, plain.length());

	for (int i = 1; i <= 8; i++) {
	    L[i] = R[i-1];
	    R[i] = xorBits(L[i-1], fFunction(xorBits(eFunction(R[i-1]), rotKey(KEY, i))));
	}

	return R[8]+L[8];
    }

    private static String fFunction(String in) {
	int s1Index =  Integer.parseInt(in.substring(0, in.length()/2), 2);
	int s2Index =  Integer.parseInt(in.substring(in.length()/2, in.length()), 2);
	return S1[s1Index] + S2[s2Index];
    }

    private static String rotKey(String key, int i) {
	StringBuilder result = new StringBuilder();
	result.append(key.substring(i, key.length()));
	result.append(key.substring(0, i));
	return result.toString();
    }

    private static String eFunction(String in) {
	StringBuilder result = new StringBuilder();
	for (int i = 0; i < PERMUTACJA.length(); i++) {
	    result.append(in.charAt(Integer.parseInt(PERMUTACJA.substring(i, i+1))-1));
	}
	return result.toString();
    }

    private static String xorBits(String a, String b) {
	StringBuilder result = new StringBuilder();
	for (int i = 0; i < a.length(); i++) {
	    result.append((int) a.charAt(i) ^ (int) b.charAt(i));
	}
	return result.toString();
    }


}
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.IOException;
import java.awt.Color;
import java.awt.image.*;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import javax.imageio.ImageIO;

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
public class tryby {
	
    
    private static final String FILE_PLAIN = "plain.bmp";
    private static final String FILE_ECB_CRYPT = "ecb_crypto.bmp";
    private static final String FILE_CBC_CRYPT = "cbc_crypto.bmp";
    private static final String FILE_KEY = "key.txt";
    //private static final String FILE_ANALYZE = "analyze.txt";

    private static final String PERMUTACJA = "12434356";
    private static String KEY;

    private static String[] S1 = {"101", "010", "001", "110", "011", "100", 
    "111", "000", "001", "100", "110", "010", "000", "111", "101", "011"};
    private static String[] S2 = {"100", "000", "110", "101", "111", "001",
    "011", "010", "101", "011", "000", "111", "110", "010", "001", "100"};
    
    
    public static void main(String[] args) {
    	KEY = pliki.readFile(FILE_KEY).get(0);
    	if (args.length==0){
    		ECB_encrypt();
    		CBC_encrypt();
    	} else if (args[0].equals("-ecb")) {
    		ECB_encrypt();
    	} else if (args[0].equals("-cbc")) {
    		CBC_encrypt();
    	}
    }
    
    
    public static void CBC_encrypt(){
    	int x,y;
    	BufferedImage wej = null;
    	BufferedImage wyj = null;
    	Color color,color2;
    	String pblok = "100111010110";
    	
    	try {
    		wej = ImageIO.read(new File(FILE_PLAIN));
    	} catch (IOException e) {}
    	
    	x = wej.getWidth(); y = wej.getHeight();
    	if ( x%3 != 0) x += (3 - (x%3));
    	if ( y%4 != 0) y += (4 - (y%4));
    	wyj = new BufferedImage(x, y, BufferedImage.TYPE_INT_RGB);
    	
    	color = new Color(wej.getRGB(0, 0));
    	StringBuilder blok ; String eblok;
    	
    	for(int i=0; i<wej.getWidth(); i+=3){
    		for (int j=0; j<wej.getHeight(); j+=4){
    			blok = new StringBuilder();
    			for (int k=0;k<12;k++){
    				x = i+(k%3); y = j+(int)(k/3);
    				if (x < wej.getWidth() && y < wej.getHeight()){
    				color = new Color(wej.getRGB(x,y));
    				if (color.getRed()>127) blok.append('1'); 
    				else blok.append('0');
    				} else blok.append('0');
    			}
    		eblok = encrypt(xorBits(pblok, blok.toString()));
    			pblok = eblok;
    			for (int k=0;k<12;k++){
    				color = new Color(255,255,255);
    				color2 = new Color(0,0,0);
    				x = i+(k%3); y = j+(int)(k/3);
    				if (eblok.charAt(k) == '1') wyj.setRGB(x, y, color.getRGB());
    				else wyj.setRGB(x, y, color2.getRGB());
    			}
    		}
    	}
    	try { 
    		  File outputfile = new File(FILE_CBC_CRYPT); 
    		  ImageIO.write(wyj, "bmp", outputfile); 
    		} catch (IOException e) {} 
    	System.out.println("CBC zakonczone");
    }
    
    public static void ECB_encrypt(){
    	int x,y;
    	BufferedImage wej = null;
    	BufferedImage wyj = null;
    	Color color,color2;
    	
    	try {
    		wej = ImageIO.read(new File(FILE_PLAIN));
    	} catch (IOException e) {}
    	
    	x = wej.getWidth(); y = wej.getHeight();

    	if ( x%3 != 0) x += (3 - (x%3));
    	if ( y%4 != 0) y += (4 - (y%4));
    	wyj = new BufferedImage(x, y, BufferedImage.TYPE_INT_RGB);
    	
    	color = new Color(wej.getRGB(0, 0));
    	StringBuilder blok ; String eblok;
    	
    	for(int i=0; i<wej.getWidth(); i+=3){
    		for (int j=0; j<wej.getHeight(); j+=4){
    			blok = new StringBuilder();
    			for (int k=0;k<12;k++){
    				x = i+(k%3); y = j+(int)(k/3);
    				if (x < wej.getWidth() && y < wej.getHeight()){
    				color = new Color(wej.getRGB(x,y));
    				if (color.getRed()>127) blok.append('1'); 
    				else blok.append('0');
    				} else blok.append('0');
    			}
   
    			eblok = encrypt(blok.toString());
    			for (int k=0;k<12;k++){
    				color = new Color(255,255,255);
    				color2 = new Color(0,0,0);
    				x = i+(k%3); y = j+(int)(k/3);
    				if (eblok.charAt(k) == '1') wyj.setRGB(x, y, color.getRGB());
    				else wyj.setRGB(x, y, color2.getRGB());
    			}
    		}
    	}
    	try { 
    		  File outputfile = new File(FILE_ECB_CRYPT); 
    		  ImageIO.write(wyj, "bmp", outputfile); 
    		} catch (IOException e) {} 
    	System.out.println("ECB zakonczone.");
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

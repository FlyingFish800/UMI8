package Microcode;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Scanner;

public class MicrocodeGenerator {
    // Ctrl lines
    public static final int II =  0b00000000;  // Instruction in
    public static final int MIH = 0b00100000;  // Memory in high
    public static final int MIL = 0b01000000;  // Memory in low
    public static final int PIH = 0b01100000;  // Pointer in high
    public static final int PIL = 0b10000000;  // Pointer in low
    public static final int AI  = 0b10100000;  // A in
    public static final int BI  = 0b11000000;  // B in
    public static final int WR  = 0b11100000;  // Memory Write

    public static final int BO  = 0b00000000;  // Mem out low
    public static final int EOS = 0b00000100;  // Mem out High
    public static final int POL = 0b00001000;  // Pointer out Low
    public static final int POH = 0b00001100;  // Pointer out High
    public static final int AO  = 0b00010000;  // A out
    public static final int EO  = 0b00010100;  // Sum out
    public static final int RD  = 0b00011000;  // Memory Read
    public static final int CR  = 0b00011100;  // Count reset

    public static final int ADD = 0b00000000;  // ADD
    public static final int CE  = 0b00000001;  // Count enable
    public static final int INC = 0b00000010;  // Increment/ADD
    public static final int RSA= 0b00000011;  // Reset A

    // How many microsteps
    public static final int MICROSTEPS = 32;

    // Hold code
    public Byte[] microcode = new Byte[8192];
    public String sourceFile;

    public static void main(String[] args) {
        new MicrocodeGenerator();
    } // end main

    public MicrocodeGenerator(){
        sourceFile = readFile("microcode.mc");
        generateBinary();
        outputBinary("microcode.bin");
    } // end class

    public void generateBinary() {
        String[] lines = sourceFile.split("\n");  // Split source code by newlines
        for (String line : lines) { // For every line

            int wordsUsed = 0; // Track amount of words to pad out to 16 microsteps
            boolean comment = false;
            int index = 0;
            int flags = 0;
            String[] words = line.split(",");   // Split by commas for µcode 'word'

            for (int w = 0; w < words.length; w++) {    // For every 'word' (also reffered to as section)

                String word = words[w]; // Get word
                wordsUsed++;
                int binaryWord = 0b00000000;

                String[] controlSeq = word.split(" "); // get ctrl lines

                for (int i = 0; i < controlSeq.length; i++) { // For every CTRL line in section

                    String ctrlCode = controlSeq[i]; // Get ctrl code

                    if(ctrlCode.contains(";")) {
                        comment = true; // Set comment if it is one
                        if(binaryWord!=0) insertChunkPortion(binaryWord, index, wordsUsed-1, flags); // Still print if ; at end of current section
                    }

                    if(!comment && ctrlCode.length() >= 1){ // If it isn't a comment or nonexistant
                        //System.out.println(ctrlCode);
                        if(ctrlCode.charAt(0) == 'o'){  // Parse o00***, section, the opcode and flags
                            index = Integer.parseInt(ctrlCode.substring(1, ctrlCode.length()-3), 8);
                            flags = parseFlags(ctrlCode.substring(ctrlCode.length()-3, ctrlCode.length()));
                            System.out.println(index);
                            wordsUsed--;
                        } else{ 
                            System.out.println("Word");
                            switch (ctrlCode) {
                                // DST
                                case "II":
                                    binaryWord |= II;
                                    break;

                                case "MIH":
                                    binaryWord |= MIH;
                                    break;
                                    
                                case "MIL":
                                    binaryWord |= MIL;
                                    break;

                                case "PIH":
                                    binaryWord |= PIH;
                                    break;
                                    
                                case "PIL":
                                    binaryWord |= PIL;
                                    break;
                                    
                                case "AI":
                                    binaryWord |= AI;
                                    break;

                                case "BI":
                                    binaryWord |= BI;
                                    break;
                                    
                                case "WR":
                                    binaryWord |= WR;
                                    break;

                                // SRC
                                case "BO":
                                    binaryWord |= BO;
                                    break;

                                case "EOS":
                                    binaryWord |= EOS;
                                    break;
                                    
                                case "POL":
                                    binaryWord |= POL;
                                    break;

                                case "POH":
                                    binaryWord |= POH;
                                    break;
                                    
                                case "AO":
                                    binaryWord |= AO;
                                    break;
                                    
                                case "EO":
                                    binaryWord |= EO;
                                    break;

                                case "RD":
                                    binaryWord |= RD;
                                    break;
                                    
                                case "CR":
                                    binaryWord |= CR;
                                    break;

                                // ALU
                                case "ADD":
                                    binaryWord |= ADD;
                                    break;
                                    
                                case "CE":
                                    binaryWord |= CE;
                                    break;

                                case "RSA":
                                    binaryWord |= RSA;
                                    break;
                                    
                                case "INC":
                                    binaryWord |= INC;
                                    break;
                            
                                default:
                                    System.err.println("Unknown Ctrl line '" + ctrlCode + "'");
                                    System.exit(1);
                                    break;
                            }
                        }
                    } // end if

                } // end for (ctrlSeq)
                // Keep in mind comment is technically in strlseq. See above
                if(!comment && wordsUsed>0) insertChunkPortion(binaryWord, index, wordsUsed-1, flags);

            } // end for (word)

        } // end for (chunks)

    } // end generateBinary

    // Parse flag state following opcode in microcode data (ZNC)
    private int parseFlags(String flagStates) {
        // Track flags state
        int flags = 0;

        if(flagStates.equals("***")) return 0b1000000; // If wildcard, return special 8 case where all flags valid
        if(flagStates.charAt(0) == '+') flags |= 0b100; // If Zero set, set Z bit
        if(flagStates.charAt(1) == '+') flags |= 0b010; // If Negative set, set N bit
        if(flagStates.charAt(2) == '+') flags |= 0b001; // If Carry set, set C bit
        // Track Wildcards
        if(flagStates.charAt(0) == '*') flags |= 0b100000; // If Zero set, set Z bit
        if(flagStates.charAt(1) == '*') flags |= 0b010000; // If Negative set, set N bit
        if(flagStates.charAt(2) == '*') flags |= 0b001000; // If Carry set, set C bit

        return flags;
    } // end parseFlags

    // Index should be IIIIIIFFFCCCC ; I = Instruction, F = flag, C = counter
    public void insertChunkPortion(int code, int opcode, int step, int flagsAndWilds){ // TODO: implement *** (all flag configs) for flags = 8
        System.out.printf("%s\n", Integer.toBinaryString(code));
        int index = 0;
        int flags = flagsAndWilds & 0b111;
        // Handle all flag cases
        if(flagsAndWilds == 0b1000000){ // wildcard case
            for (int i = 0; i < 8; i++) {
                index = (((opcode & 0b11111) << 8) | ((i & 0b111) << 5) | (step & 0b11111)); // *2 because we UMI used 2 byte control words
                insertChunkAtIndex(code, index); // insert
            } // end for
        } else { // If not wildcards. Can handle multiple flags se
            int wilds = (flagsAndWilds & 0b111000) >> 3;
            for (int i = 0; i < 8; i++) {
                System.out.println("Flags: "+Integer.toBinaryString(flags)+ "    Wilds: "+Integer.toBinaryString(((i & wilds) | flags)));
                index = (((opcode & 0b11111) << 8) | (((i & wilds) | flags) << 5) | (step & 0b11111));
                insertChunkAtIndex(code, index); // insert
            } // end for
        } // end else
    } // end insertChunkPortion

    // Once index has been calculated, insert into ucode
    public void insertChunkAtIndex (int code, int index){
        //int index = (opcode*MICROSTEPS+step)*2; // TODO: FLAGS
        String instruction = String.format("%02X", code); // TODO: If it doesn't work, therse are reversed
        //System.out.println(index + ": " + Integer.toBinaryString(code));
        //System.out.println((index & 0b11100000)); Check flags (5 zeros because of multiply by two)
        microcode[index] = (byte) ((Character.digit(instruction.charAt(0), 16) << 4) + Character.digit(instruction.charAt(1), 16));
    } // end insertChunkAtIndex

    public String readFile (String path){    // Load file

        File file = new File("./Microcode/" + path);    // Create refernce to file
        Scanner sc; // Define scanner
        String fileContents = "";   // Define string to hold file contents

        try {
            sc = new Scanner(file); // Try to create scanner
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            System.exit(1);
            return null;
        }

        while (sc.hasNextLine()){
            fileContents += sc.nextLine() + "\n"; // Append text to temp variable to hold all source code
        }

        sc.close(); // Close scanner

        return fileContents;    // Return file contents

    } // end readFile

    public void outputBinary(String path){
        OutputStream opStream = null;
        try {
            // TODO: 0-127 unchanged, 128 = -128
            File myFile = new File("./Microcode/" + path);
            // check if file exist, otherwise create the file before writing
            if (!myFile.exists()) {
                myFile.createNewFile();
            }
            opStream = new FileOutputStream(myFile);
            for (Byte codeBit : microcode) {
                if(codeBit!=null)opStream.write(codeBit);
                else opStream.write(0x0);
            }
            opStream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        } finally{
            try{
                if(opStream != null) opStream.close();
            } catch(Exception ex){
                 
            }

        }
        
    } // end outputBinary

} // end Class

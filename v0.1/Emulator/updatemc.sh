cd ../Microcode
javac *.java
java -cp MicrocodeGenerator MicrocodeGenerator.java
cp microcode.bin ../Emulator/microcode.bin 
cd ../Emulator
cd ..
cp microcode.mc Microcode/microcode.mc
javac *.java
java -cp MicrocodeGenerator MicrocodeGenerator.java
cp Microcode/microcode.bin Emulator/microcode.bin 
cd Emulator
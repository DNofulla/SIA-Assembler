#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *words[5];

/* Take a string. Split it into different words, putting them in the words array. For example:
 * This is a string
 * Becomes:
 * words[0] = This
 * words[1] = is
 * words[2] = a
 * words[3] = string
 * Only safe when words is big enough...
 */
void getWords(char * string) { 
	printf ("input: %s\n", string);
	int curWord = 0;
	char * cur = string;
	words[curWord] = string;
	while (*cur != 0) {
		if (*cur == '\n' || *cur == '\r'){
			*cur = ' ';
		}

		if (*cur == ' ') {
			*cur = 0; // replace space with 0
			curWord++;
			words[curWord] = cur + 1; // set the start of the next word to the character after this one
		} 
		cur++;
	} 
	
	for (int i = 0;i < curWord; i++) {
		printf("word %d = %s\n", i, words[i]);
	}
}

// takes a string and returns register number or -1 if the string doesn't start with "r" or "R"
int getRegister (char * string) {
	if (string[0] != 'R' && string[0] != 'r') {
		return -1;
	}
	return atoi(string + 1);
}


int assembleLine(char * string, char * bytes) {


	getWords(string);

	/* 3R STYLE INSTRUCTIONS: FOLLOW THE SAME STYLE AS THE "ADD" INSTRUCTION, CHANGE OPCODE ONLY, HALT IS 0x00 for BOTH BYTES AS REQUIRED */
	/* WE USE RETURN 2 TO RETURN 2 BYTES */
	if (strcmp(words[0] ,"add") == 0) { // ADD INSTRUCTION OPCODE 1 or 0x1

		bytes[0] = 0x10 | getRegister(words[1]); // The opcode is 1 so 0x1, and then for the 2nd part of the byte (4 bits), we get the register
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]); // We shift the 2nd register to the top 4 bits of this byte so we can get the 3rd register as the bottom 4 bits
		return 2; // RETURN 2 BYTES

	} else if (strcmp(words[0], "and") == 0){  // AND INSTRUCTION OPCODE 2 or 0x2

		bytes[0] = 0x20 | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;

	} else if (strcmp(words[0], "divide") == 0){ // DIVIDE INSTRUCTION OPCODE 3 or 0x3

		bytes[0] = 0x30 | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;

	} else if (strcmp(words[0], "halt") == 0){ // HALT INSTRUCTION OPCODE 0 or 0x0

		// BOTH BYTES ARE ZEROS, CPU HALTS
		bytes[0] = 0x00;
		bytes[1] = 0x00;
		return 2;

	} else if (strcmp(words[0], "multiply") == 0){ // MULTIPLY INSTRUCTION OPCODE 4 or 0x4

		bytes[0] = 0x40 | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;

	} else if (strcmp(words[0], "or") == 0){ // OR INSTRUCTION OPCODE 6 or 0x6
		
		bytes[0] = 0x60 | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;

	} else if (strcmp(words[0], "subtract") == 0){ // SUBTRACT INSTRUCTION OPCODE 5 or 0x5
		bytes[0] = 0x50 | getRegister(words[1]);
		bytes[1] = (getRegister(words[2]) << 4) | getRegister(words[3]);
		return 2;
		
	} 
	
	
	
	/* BR1 STYLE BRANCH INSTRUCTIONS: FOLLOW THE SAME STYLE AS THE "BRANCHIFLESS" INSTRUCTION, CHANGE BRANCH TYPE NUMBER ONLY */
	/* BR1 STYLE BRANCH TYPES RANGE FROM 0 to 5, OPCODE IS ALWAYS 7 */
	/* WE USE RETURN 4 TO RETURN 4 BYTES */
	else if (strcmp(words[0], "branchIfLess") == 0){ // BRANCH IF < INSTRUCTION, OPCODE 7 BRANCH TYPE 0

		bytes[0] = 0x70; // OPCODE 7, BRANCH TYPE 0. Adjust this accordingly with the rest of the branch types and branches
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]); // We shift 4 bits to the left of the first register, and then with the remaining 4 bits we get the 2nd register. Much like in the 3R style instructions
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF; // We shift 8 bytes left so we get the top bits of the address offset on the third byte
		bytes[3] = (atoi(words[3]) & 0xFF); // We get the rest 8 bottom bits of the address offset (No need to shift) 
		return 4; // RETURN 4 BYTES

	} else if (strcmp(words[0], "branchIfLessOrEqual") == 0){ // BRANCH IF <= INSTRUCTION OPCODE, 7 BRANCH TYPE 1

		bytes[0] = 0x71;
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF;
		bytes[3] = (atoi(words[3]) & 0xFF);
		return 4;

	} else if (strcmp(words[0], "branchIfEqual") == 0){ // BRANCH IF == INSTRUCTION, OPCODE 7 BRANCH TYPE 2

		bytes[0] = 0x72;
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF;
		bytes[3] = (atoi(words[3]) & 0xFF);
		return 4;

	} else if (strcmp(words[0], "branchIfNotEqual") == 0){ // BRANCH IF != INSTRUCTION, OPCODE 7 BRANCH TYPE 3

		bytes[0] = 0x73;
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF;
		bytes[3] = (atoi(words[3]) & 0xFF);
		return 4;

	} else if (strcmp(words[0], "branchIfGreater") == 0){ // BRANCH IF > INSTRUCTION, OPCODE 7 BRANCH TYPE 4

		bytes[0] = 0x74;
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF;
		bytes[3] = (atoi(words[3]) & 0xFF);
		return 4;

	} else if (strcmp(words[0], "branchIfGreaterOrEqual") == 0){ // BRANCH IF >= INSTRUCTION, OPCODE 7 BRANCH TYPE 5

		bytes[0] = 0x75;
		bytes[1] = (getRegister(words[1]) << 4) | getRegister(words[2]);
		
		// 16 bits aka 2 bytes of address offset
		bytes[2] = (atoi(words[3]) >> 8) & 0xFF;
		bytes[3] = (atoi(words[3]) & 0xFF);
		return 4;

	} 
	
	
	
	
	
	/* BR2 STYLE BRANCH INSTRUCTIONS: FOLLOW THE SAME STYLE AS THE "BRANCHIFLESS" INSTRUCTION, CHANGE BRANCH TYPE NUMBER ONLY */
	/* BR2 STYLE IS ONLY USED BY THE "CALL" AND "JUMP" INSTRUCTIONS, WHOSE OPCODE IS 7 FOR BOTH, AND BRANCH TYPE 6 FOR "CALL" AND BRANCH TYPE 7 FOR JUMP */
	/* ADDRESS IS 24 BITS, Split into 8 upper and 16 bottom bits */
	else if (strcmp(words[0], "call") == 0){

		bytes[0] = 0x76; // OPCODE 7 BRANCH TYPE 6

		// Top 8 bits aka 1 byte of address
		bytes[1] = (atoi(words[1]) >> 16) & 0xFF; // SHIFT 16 bits right so we get the top 8 bits of the 24 bit address for this byte
		
		// Bottom 16 bits aka 2 bytes of address
		bytes[2] = (atoi(words[1]) >> 8) & 0xFF; // SHIFT 8 bits right so we get the middle 8 bits of the 24 bit address for this byte
		bytes[3] = (atoi(words[1]) & 0xFF); // No Shift of bits so we get the bottom 8 bits of the 24 bit address for this byte
		return 4; // RETURN 4 BYTES
		
	} else if (strcmp(words[0], "jump") == 0){

		bytes[0] = 0x77; // OPCODE 7 BRANCH TYPE 7

		// Top 8 bits aka 1 byte of address
		bytes[1] = (atoi(words[1]) >> 16) & 0xFF; // SHIFT 16 bits right so we get the top 8 bits of the 24 bit address for this byte
		
		// Bottom 16 bits aka 2 bytes of address
		bytes[2] = (atoi(words[1]) >> 8) & 0xFF; // SHIFT 8 bits right so we get the middle 8 bits of the 24 bit address for this byte
		bytes[3] = (atoi(words[1]) & 0xFF); // No Shift of bits so we get the bottom 8 bits of the 24 bit address for this byte
		return 4; // RETURN 4 BYTES

	} 
	
	
	
	
	/* LS STYLE INSTRUCTIONS: LOAD HAS OPCODE 8 WHILE STORE HAS OPCODE 9, THEY WORK THE SAME EVERYWHERE ELSE */
	else if (strcmp(words[0], "load") == 0){

		bytes[0] = 0x80 | (getRegister(words[1])); // OPCODE 8 so 0x8 with the second part (bottom 4 bits) just gets the register to load (bottom 4 bits, no need to shift anything)
		bytes[1] = ((getRegister(words[2])) << 4) | ((getRegister(words[3])) & 0xF);  // Get the address register and shift by 4 to the left (setting it as the top 4 bits), and then for the bottom 4 bits we have the address offset, also using only 0xF instead of 0xFF because its 4 bits
		return 2; // RETURN 2 BYTES

	} else if (strcmp(words[0], "store") == 0){

		bytes[0] = 0x90 | (getRegister(words[1])); // OPCODE 9 so 0x9 with the second part (bottom 4 bits) just gets the register to store (bottom 4 bits, no need to shift anything)
		bytes[1] = ((getRegister(words[2])) << 4) | ((getRegister(words[3])) & 0xF); // Get the address register and shift by 4 to the left (setting it as the top 4 bits), and then for the bottom 4 bits we have the address offset, also using only 0xF instead of 0xFF because its 4 bits
		return 2; // RETURN 2 BYTES

	} 
	
	
	
	
	/* STACK STYLE INSTRUCTIONS: ALL HAVE OPCODE 10 or 0xA */
	else if (strcmp(words[0], "pop") == 0){
		
		bytes[0] = 0xA0 | (getRegister(words[1])); // OPCODE 10 so 0xA, and the second part is the register we are popping from the stack
		bytes[1] = (2 << 6); // the top 2 bits of this byte must be 10 in binary, which is 2 in decimal, hence 2 << 6, the rest of the bits are unused, so we just leave them as they are (set as 0)
		return 2; // RETURN 2 BYTES

	} else if (strcmp(words[0], "push") == 0){

		bytes[0] = 0xA0 | (getRegister(words[1])); // OPCODE 10 so 0xA, and the second part is the register we are pushing to the stack
		bytes[1] = (1 << 6); // the top 2 bits of this byte must be 01 in binary, which is 1 in decimal, hence 1 << 6, the rest of the bits are unused, so we just leave them as they are (set as 0)
		return 2; // RETURN 2 BYTES

	} else if (strcmp(words[0], "return") == 0){

		// OPCODE 10 so 0xA. The rest of the bits and bytes are all set to 0. so 0xA000 is the only possibility for the return instruction
		bytes[0] = 0xA0; 
		bytes[1] = 0x00;
		return 2; // RETURN 2 BYTES

	} 
	
	
	/* MOVE STYLE INSTRUCTION: USED FOR BOTH THE "MOVE" AND "INTERRUPT" INSTRUCTIONS, OPCODES ARE 11 / 0xB and 12 / 0xC RESPECTIVELY, "INTERRUPT" INSTRUCTION IGNORED REGISTER (Bottom 4 bits of first byte set as 0 by default) */
	else if (strcmp(words[0], "move") == 0){

		bytes[0] = 0xB0 | getRegister(words[2]); // OPCODE 11 so 0xB, and then for the bottom 4 bits of this byte, we get the register
        bytes[1] = atoi(words[1]); // This byte consists of the immediate signed value we are moving to the register, we just convert the string to a number.
		return 2; // RETURN 2 BYTES

	} else if (strcmp(words[0], "interrupt") == 0){

		bytes[0] = 0xC0; // OPCODE 12 so 0xC, for interrupt there is no register needed, it is ignored, so the byte is 0xC0 by default
		bytes[1] = atoi(words[1]); // This byte consists of the immediate signed value we are interrupting with, we just convert the string to a number. REGISTER IS IGNORED
		return 2; // RETURN 2 BYTES

	} 
	
	/* Display */
	else {
		printf("Invalid Instruction: %s", words[0]);
		return 0;
	}

}

int main (int argc, char **argv)  {

	if (argc != 3)  {
		printf("Assemble InputFile OutputFile\n"); 
		exit(1); 
	}

	FILE * in = fopen(argv[1], "r");

	if (in == NULL) { 
		printf("Unable to open input file\n"); 
		exit(1); 
	}

	FILE *out = fopen(argv[2],"wb");

	if (out == NULL) { 
		printf("Unable to open output file\n"); 
		exit(1); 
	}

	char bytes[4];
	char inputLine[100];
	while (!feof(in)) {
		if (NULL != fgets(inputLine, 100, in)) {
			int outSize = assembleLine(inputLine, bytes);
			fwrite(bytes, outSize, 1, out);
		}
	}
	fclose(in);
	fclose(out);
}
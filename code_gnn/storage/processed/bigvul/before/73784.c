void zrtpMessageSetHeader(uint8_t *outputBuffer, uint16_t messageLength, uint8_t messageType[8]) {
	 
	outputBuffer[0] = 0x50;
	outputBuffer[1] = 0x5a;

	 
	outputBuffer[2] = (uint8_t)((messageLength>>10)&0x00FF);
	outputBuffer[3] = (uint8_t)((messageLength>>2)&0x00FF);

	 
	memcpy(outputBuffer+4, messageType, 8);
}

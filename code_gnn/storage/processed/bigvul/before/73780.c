int bzrtp_packetUpdateSequenceNumber(bzrtpPacket_t *zrtpPacket, uint16_t sequenceNumber) {
	uint32_t CRC;
	uint8_t *CRCbuffer;

	if (zrtpPacket == NULL) {
		return BZRTP_BUILDER_ERROR_INVALIDPACKET;
	}

	if (zrtpPacket->packetString == NULL) {
		return BZRTP_BUILDER_ERROR_INVALIDPACKET;
	}
	 
	zrtpPacket->sequenceNumber = sequenceNumber;

	 
	*(zrtpPacket->packetString+2)= (uint8_t)((sequenceNumber>>8)&0x00FF);
	*(zrtpPacket->packetString+3)= (uint8_t)(sequenceNumber&0x00FF);


	 
	CRC = bzrtp_CRC32(zrtpPacket->packetString, zrtpPacket->messageLength+ZRTP_PACKET_HEADER_LENGTH);
	CRCbuffer = (zrtpPacket->packetString)+(zrtpPacket->messageLength)+ZRTP_PACKET_HEADER_LENGTH;
	*CRCbuffer = (uint8_t)((CRC>>24)&0xFF);
	CRCbuffer++;
	*CRCbuffer = (uint8_t)((CRC>>16)&0xFF);
	CRCbuffer++;
	*CRCbuffer = (uint8_t)((CRC>>8)&0xFF);
	CRCbuffer++;
	*CRCbuffer = (uint8_t)(CRC&0xFF);

	return 0;
}

int bzrtp_packetBuild(bzrtpContext_t *zrtpContext, bzrtpChannelContext_t *zrtpChannelContext, bzrtpPacket_t *zrtpPacket, uint16_t sequenceNumber) {
	
	int i;
	uint8_t *messageTypeString;
	uint8_t *messageString = NULL;  
	uint8_t *MACbuffer = NULL;  
	   
	uint8_t *MACkey = NULL;

	 
	if (zrtpPacket==NULL) {
		return BZRTP_BUILDER_ERROR_INVALIDPACKET;
	}

	 
	messageTypeString = messageTypeInttoString(zrtpPacket->messageType);
	if (messageTypeString == NULL) {
		return BZRTP_BUILDER_ERROR_INVALIDMESSAGETYPE;
	}

	 
	switch (zrtpPacket->messageType) {
		case MSGTYPE_HELLO : 
			{
				bzrtpHelloMessage_t *messageData;

				 
				if (zrtpPacket->messageData == NULL) {
					return BZRTP_BUILDER_ERROR_INVALIDMESSAGE;
				}
				messageData = (bzrtpHelloMessage_t *)zrtpPacket->messageData;

				 
				zrtpPacket->messageLength = ZRTP_HELLOMESSAGE_FIXED_LENGTH + 4*((uint16_t)(messageData->hc)+(uint16_t)(messageData->cc)+(uint16_t)(messageData->ac)+(uint16_t)(messageData->kc)+(uint16_t)(messageData->sc));

				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+zrtpPacket->messageLength+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
				 
				messageString = zrtpPacket->packetString + ZRTP_PACKET_HEADER_LENGTH + ZRTP_MESSAGE_HEADER_LENGTH;

				 
				memcpy(messageString, messageData->version, 4);
				messageString += 4;
				memcpy(messageString, messageData->clientIdentifier, 16);
				messageString += 16;
				memcpy(messageString, messageData->H3, 32);
				messageString += 32;
				memcpy(messageString, messageData->ZID, 12);
				messageString += 12;
				*messageString = ((((messageData->S)&0x01)<<6) | (((messageData->M)&0x01)<<5) | (((messageData->P)&0x01)<<4))&0x70;
				messageString += 1;
				*messageString = (messageData->hc)&0x0F;
				messageString += 1;
				*messageString = (((messageData->cc)<<4)&0xF0) | ((messageData->ac)&0x0F) ;
				messageString += 1;
				*messageString = (((messageData->kc)<<4)&0xF0) | ((messageData->sc)&0x0F) ;
				messageString += 1;

				 
				for (i=0; i<messageData->hc; i++) {
					cryptoAlgoTypeIntToString(messageData->supportedHash[i], messageString);
					messageString +=4;
				}
				for (i=0; i<messageData->cc; i++) {
					cryptoAlgoTypeIntToString(messageData->supportedCipher[i], messageString);
					messageString +=4;
				}
				for (i=0; i<messageData->ac; i++) {
					cryptoAlgoTypeIntToString(messageData->supportedAuthTag[i], messageString);
					messageString +=4;
				}
				for (i=0; i<messageData->kc; i++) {
					cryptoAlgoTypeIntToString(messageData->supportedKeyAgreement[i], messageString);
					messageString +=4;
				}
				for (i=0; i<messageData->sc; i++) {
					cryptoAlgoTypeIntToString(messageData->supportedSas[i], messageString);
					messageString +=4;
				}

				 
				MACbuffer = messageString;
				MACkey = zrtpChannelContext->selfH[2];  

			}
			break;  

		case MSGTYPE_HELLOACK : 
			{
				 
				zrtpPacket->messageLength = ZRTP_HELLOACKMESSAGE_FIXED_LENGTH;

				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+ZRTP_HELLOACKMESSAGE_FIXED_LENGTH+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
			}
			break;  

		case MSGTYPE_COMMIT :
			{
				bzrtpCommitMessage_t *messageData;
				uint16_t variableLength = 0;

				 
				if (zrtpPacket->messageData == NULL) {
					return BZRTP_BUILDER_ERROR_INVALIDMESSAGE;
				}

				messageData = (bzrtpCommitMessage_t *)zrtpPacket->messageData;

				 
				switch(messageData->keyAgreementAlgo) {
					case ZRTP_KEYAGREEMENT_DH2k :
					case ZRTP_KEYAGREEMENT_EC25 :
					case ZRTP_KEYAGREEMENT_DH3k :
					case ZRTP_KEYAGREEMENT_EC38 :
					case ZRTP_KEYAGREEMENT_EC52 :
						variableLength = 32;  
						break;
					case ZRTP_KEYAGREEMENT_Prsh :
						variableLength = 24;  
						break;
					case ZRTP_KEYAGREEMENT_Mult :
						variableLength = 16;  
						break;
					default:
						return BZRTP_BUILDER_ERROR_INVALIDMESSAGE;
				}
				zrtpPacket->messageLength = ZRTP_COMMITMESSAGE_FIXED_LENGTH + variableLength;
				
				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+zrtpPacket->messageLength+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
				 
				messageString = zrtpPacket->packetString + ZRTP_PACKET_HEADER_LENGTH + ZRTP_MESSAGE_HEADER_LENGTH;

				 
				memcpy(messageString, messageData->H2, 32);
				messageString += 32;
				memcpy(messageString, messageData->ZID, 12);
				messageString += 12;
				cryptoAlgoTypeIntToString(messageData->hashAlgo, messageString);
				messageString += 4;
				cryptoAlgoTypeIntToString(messageData->cipherAlgo, messageString);
				messageString += 4;
				cryptoAlgoTypeIntToString(messageData->authTagAlgo, messageString);
				messageString += 4;
				cryptoAlgoTypeIntToString(messageData->keyAgreementAlgo, messageString);
				messageString += 4;
				cryptoAlgoTypeIntToString(messageData->sasAlgo, messageString);
				messageString += 4;

				 
				if ((messageData->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Prsh) || (messageData->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Mult)) {
						memcpy(messageString, messageData->nonce, 16);
						messageString += 16;

					 
					if (messageData->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Prsh) {
						memcpy(messageString, messageData->keyID, 8);
						messageString +=8;
					}
				} else {  
					memcpy(messageString, messageData->hvi, 32);
					messageString +=32;
				}
			
				 
				MACbuffer = messageString;
				MACkey = zrtpChannelContext->selfH[1];  
			}
			break;  

		case MSGTYPE_DHPART1 :
		case MSGTYPE_DHPART2 :
			{
				bzrtpDHPartMessage_t *messageData;
				uint16_t pvLength;

				 
				if (zrtpPacket->messageData == NULL) {
					return BZRTP_BUILDER_ERROR_INVALIDMESSAGE;
				}

				messageData = (bzrtpDHPartMessage_t *)zrtpPacket->messageData;

				 
				pvLength = computeKeyAgreementPrivateValueLength(zrtpChannelContext->keyAgreementAlgo);
				if (pvLength==0) {
					return BZRTP_BUILDER_ERROR_INVALIDCONTEXT;
				}
				zrtpPacket->messageLength = ZRTP_DHPARTMESSAGE_FIXED_LENGTH + pvLength;
				
				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+zrtpPacket->messageLength+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
				 
				messageString = zrtpPacket->packetString + ZRTP_PACKET_HEADER_LENGTH + ZRTP_MESSAGE_HEADER_LENGTH;

				 
				memcpy(messageString, messageData->H1, 32);
				messageString += 32;
				memcpy(messageString, messageData->rs1ID, 8);
				messageString += 8;
				memcpy(messageString, messageData->rs2ID, 8);
				messageString += 8;
				memcpy(messageString, messageData->auxsecretID, 8);
				messageString += 8;
				memcpy(messageString, messageData->pbxsecretID, 8);
				messageString += 8;
				memcpy(messageString, messageData->pv, pvLength);
				messageString += pvLength;

				 
				MACbuffer = messageString;
				MACkey = zrtpChannelContext->selfH[0];  
			}
			break;  
		
		case MSGTYPE_CONFIRM1:
		case MSGTYPE_CONFIRM2:
			{
				uint8_t *confirmMessageKey = NULL;
				uint8_t *confirmMessageMacKey = NULL;
				bzrtpConfirmMessage_t *messageData;
				uint16_t encryptedPartLength;
				uint8_t *plainMessageString;
				uint16_t plainMessageStringIndex = 0;

				 
				if (zrtpChannelContext->role == INITIATOR) {
					if ((zrtpChannelContext->zrtpkeyi == NULL) || (zrtpChannelContext->mackeyi == NULL)) {
						return BZRTP_BUILDER_ERROR_INVALIDCONTEXT;
					}
					confirmMessageKey = zrtpChannelContext->zrtpkeyi;
					confirmMessageMacKey = zrtpChannelContext->mackeyi;
				}

				if (zrtpChannelContext->role == RESPONDER) {
					if ((zrtpChannelContext->zrtpkeyr == NULL) || (zrtpChannelContext->mackeyr == NULL)) {
						return BZRTP_BUILDER_ERROR_INVALIDCONTEXT;
					}
					confirmMessageKey = zrtpChannelContext->zrtpkeyr;
					confirmMessageMacKey = zrtpChannelContext->mackeyr;
				}
				
				 
				if (zrtpPacket->messageData == NULL) {
					return BZRTP_BUILDER_ERROR_INVALIDMESSAGE;
				}

				messageData = (bzrtpConfirmMessage_t *)zrtpPacket->messageData;

				 
				zrtpPacket->messageLength = ZRTP_CONFIRMMESSAGE_FIXED_LENGTH + messageData->sig_len*4;  
				
				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+zrtpPacket->messageLength+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
				 
				messageString = zrtpPacket->packetString + ZRTP_PACKET_HEADER_LENGTH + ZRTP_MESSAGE_HEADER_LENGTH;

				 
				encryptedPartLength = zrtpPacket->messageLength - ZRTP_MESSAGE_HEADER_LENGTH - 24;  
				plainMessageString = (uint8_t *)malloc(encryptedPartLength*sizeof(uint8_t)); 

				 
				memcpy(plainMessageString, messageData->H0, 32);
				plainMessageStringIndex += 32;
				plainMessageString[plainMessageStringIndex++] = 0x00;
				plainMessageString[plainMessageStringIndex++] = (uint8_t)(((messageData->sig_len)>>8)&0x0001);
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->sig_len)&0x00FF);
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->E&0x01)<<3) | (uint8_t)((messageData->V&0x01)<<2) | (uint8_t)((messageData->A&0x01)<<1) | (uint8_t)(messageData->D&0x01) ;
				 
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->cacheExpirationInterval>>24)&0xFF);
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->cacheExpirationInterval>>16)&0xFF);
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->cacheExpirationInterval>>8)&0xFF);
				plainMessageString[plainMessageStringIndex++] = (uint8_t)((messageData->cacheExpirationInterval)&0xFF);

				if (messageData->sig_len>0) {
					memcpy(plainMessageString+plainMessageStringIndex, messageData->signatureBlockType, 4);
					plainMessageStringIndex += 4;
					 
					memcpy(plainMessageString+plainMessageStringIndex, messageData->signatureBlock, (messageData->sig_len-1)*4);
				}

				 
				zrtpChannelContext->cipherEncryptionFunction(confirmMessageKey, messageData->CFBIV, plainMessageString, encryptedPartLength, messageString+24);
				free(plainMessageString);  

				 
				zrtpChannelContext->hmacFunction(confirmMessageMacKey, zrtpChannelContext->hashLength, messageString+24, encryptedPartLength, 8, messageString);
				messageString += 8;
				 
				memcpy(messageString, messageData->CFBIV, 16);
			}
			break;  

		case MSGTYPE_CONF2ACK:
			{
				 
				zrtpPacket->messageLength = ZRTP_CONF2ACKMESSAGE_FIXED_LENGTH;

				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+ZRTP_CONF2ACKMESSAGE_FIXED_LENGTH+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
			}
			break;  

		case MSGTYPE_PINGACK:
			{
				bzrtpPingAckMessage_t *messageData;

				 
				zrtpPacket->messageLength = ZRTP_PINGACKMESSAGE_FIXED_LENGTH;

				 
				zrtpPacket->packetString = (uint8_t *)malloc((ZRTP_PACKET_HEADER_LENGTH+ZRTP_PINGACKMESSAGE_FIXED_LENGTH+ZRTP_PACKET_CRC_LENGTH)*sizeof(uint8_t));
				messageString = zrtpPacket->packetString + ZRTP_PACKET_HEADER_LENGTH + ZRTP_MESSAGE_HEADER_LENGTH;

				 
				messageData = (bzrtpPingAckMessage_t *)zrtpPacket->messageData;

				memcpy(messageString, messageData->version, 4);
				messageString += 4;
				memcpy(messageString, messageData->endpointHash, 8);
				messageString += 8;
				memcpy(messageString, messageData->endpointHashReceived, 8);
				messageString += 8;
				*messageString++ = (uint8_t)((messageData->SSRC>>24)&0xFF);
				*messageString++ = (uint8_t)((messageData->SSRC>>16)&0xFF);
				*messageString++ = (uint8_t)((messageData->SSRC>>8)&0xFF);
				*messageString++ = (uint8_t)(messageData->SSRC&0xFF);
			}
			break;  

	}

	 
	if (zrtpPacket->packetString != NULL) {
		uint32_t CRC;
		uint8_t *CRCbuffer;

		zrtpMessageSetHeader(zrtpPacket->packetString+ZRTP_PACKET_HEADER_LENGTH, zrtpPacket->messageLength, messageTypeString);

		 
		if (MACbuffer != NULL) {
			 
			 
			bctoolbox_hmacSha256(MACkey, 32, zrtpPacket->packetString+ZRTP_PACKET_HEADER_LENGTH, zrtpPacket->messageLength-8, 8, MACbuffer);
		}
	
		 
		 
		zrtpPacket->packetString[0] = 0x10;
		zrtpPacket->packetString[1] = 0x00;
		 
		zrtpPacket->packetString[2] = (uint8_t)((sequenceNumber>>8)&0x00FF);
		zrtpPacket->packetString[3] = (uint8_t)(sequenceNumber&0x00FF);
		 
		zrtpPacket->packetString[4] = (uint8_t)((ZRTP_MAGIC_COOKIE>>24)&0xFF);
		zrtpPacket->packetString[5] = (uint8_t)((ZRTP_MAGIC_COOKIE>>16)&0xFF);
		zrtpPacket->packetString[6] = (uint8_t)((ZRTP_MAGIC_COOKIE>>8)&0xFF);
		zrtpPacket->packetString[7] = (uint8_t)(ZRTP_MAGIC_COOKIE&0xFF);
		 
		zrtpPacket->packetString[8] = (uint8_t)(((zrtpPacket->sourceIdentifier)>>24)&0xFF);
		zrtpPacket->packetString[9] = (uint8_t)(((zrtpPacket->sourceIdentifier)>>16)&0xFF);
		zrtpPacket->packetString[10] = (uint8_t)(((zrtpPacket->sourceIdentifier)>>8)&0xFF);
		zrtpPacket->packetString[11] = (uint8_t)((zrtpPacket->sourceIdentifier)&0xFF);
		 
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
	} else {  
		return BZRTP_BUILDER_ERROR_UNKNOWN;
	}
}

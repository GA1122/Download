bzrtpPacket_t *bzrtp_createZrtpPacket(bzrtpContext_t *zrtpContext, bzrtpChannelContext_t *zrtpChannelContext, uint32_t messageType, int *exitCode) {
	 
	bzrtpPacket_t *zrtpPacket = (bzrtpPacket_t *)malloc(sizeof(bzrtpPacket_t));
	memset(zrtpPacket, 0, sizeof(bzrtpPacket_t));
	zrtpPacket->messageData = NULL;
	zrtpPacket->packetString = NULL;

	 
	switch(messageType) {
		case MSGTYPE_HELLO:
			{
				int i;
				bzrtpHelloMessage_t *zrtpHelloMessage = (bzrtpHelloMessage_t *)malloc(sizeof(bzrtpHelloMessage_t));
				memset(zrtpHelloMessage, 0, sizeof(bzrtpHelloMessage_t));
				 
				memcpy(zrtpHelloMessage->version, ZRTP_VERSION, 4);
				strncpy((char*)zrtpHelloMessage->clientIdentifier, ZRTP_CLIENT_IDENTIFIER, 16);
				memcpy(zrtpHelloMessage->H3, zrtpChannelContext->selfH[3], 32);
				memcpy(zrtpHelloMessage->ZID, zrtpContext->selfZID, 12);
				 
				zrtpHelloMessage->S = 0;
				zrtpHelloMessage->M = 0;
				zrtpHelloMessage->P = 0;

				 
				zrtpHelloMessage->hc = zrtpContext->hc;
				zrtpHelloMessage->cc = zrtpContext->cc;
				zrtpHelloMessage->ac = zrtpContext->ac;
				zrtpHelloMessage->kc = zrtpContext->kc;
				zrtpHelloMessage->sc = zrtpContext->sc;

				for (i=0; i<zrtpContext->hc; i++) {
					zrtpHelloMessage->supportedHash[i] = zrtpContext->supportedHash[i];
				}
				for (i=0; i<zrtpContext->cc; i++) {
					zrtpHelloMessage->supportedCipher[i] = zrtpContext->supportedCipher[i];
				}
				for (i=0; i<zrtpContext->ac; i++) {
					zrtpHelloMessage->supportedAuthTag[i] = zrtpContext->supportedAuthTag[i];
				}
				for (i=0; i<zrtpContext->kc; i++) {
					zrtpHelloMessage->supportedKeyAgreement[i] = zrtpContext->supportedKeyAgreement[i];
				}
				for (i=0; i<zrtpContext->sc; i++) {
					zrtpHelloMessage->supportedSas[i] = zrtpContext->supportedSas[i];
				}

				 
				zrtpPacket->messageData = zrtpHelloMessage;
			}
			break;  

		case MSGTYPE_HELLOACK :
			{
				 
			}
			break;  
		 
		case MSGTYPE_COMMIT :
			{
				bzrtpCommitMessage_t *zrtpCommitMessage = (bzrtpCommitMessage_t *)malloc(sizeof(bzrtpCommitMessage_t));
				memset(zrtpCommitMessage, 0, sizeof(bzrtpCommitMessage_t));
				
				 
				memcpy(zrtpCommitMessage->H2, zrtpChannelContext->selfH[2], 32);
				memcpy(zrtpCommitMessage->ZID, zrtpContext->selfZID, 12);
				zrtpCommitMessage->hashAlgo = zrtpChannelContext->hashAlgo;
				zrtpCommitMessage->cipherAlgo = zrtpChannelContext->cipherAlgo;
				zrtpCommitMessage->authTagAlgo = zrtpChannelContext->authTagAlgo;
				zrtpCommitMessage->keyAgreementAlgo = zrtpChannelContext->keyAgreementAlgo;
				zrtpCommitMessage->sasAlgo = zrtpChannelContext->sasAlgo;

				 
				if ((zrtpCommitMessage->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Prsh) || (zrtpCommitMessage->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Mult)) {
					bctoolbox_rng_get(zrtpContext->RNGContext, zrtpCommitMessage->nonce, 16);

					 
					if (zrtpCommitMessage->keyAgreementAlgo == ZRTP_KEYAGREEMENT_Prsh) {
						 
						 
						 
					}
				} else {  
					 
					 
					uint16_t DHPartMessageLength = zrtpChannelContext->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength;
					uint16_t HelloMessageLength = zrtpChannelContext->peerPackets[HELLO_MESSAGE_STORE_ID]->messageLength;
					uint16_t DHPartHelloMessageStringLength = DHPartMessageLength + HelloMessageLength;

					uint8_t *DHPartHelloMessageString = (uint8_t *)malloc(DHPartHelloMessageStringLength*sizeof(uint8_t));
					
					memcpy(DHPartHelloMessageString, zrtpChannelContext->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, DHPartMessageLength);
					memcpy(DHPartHelloMessageString+DHPartMessageLength, zrtpChannelContext->peerPackets[HELLO_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, HelloMessageLength);

					zrtpChannelContext->hashFunction(DHPartHelloMessageString, DHPartHelloMessageStringLength, 32, zrtpCommitMessage->hvi);

					free(DHPartHelloMessageString);
				}

				 
				zrtpPacket->messageData = zrtpCommitMessage;
			}
			break;  

		 
		case MSGTYPE_DHPART1 :
		case MSGTYPE_DHPART2 :
			{
				uint8_t secretLength;  
				uint8_t bctoolbox_keyAgreementAlgo = BCTOOLBOX_DHM_UNSET;
				bzrtpDHPartMessage_t *zrtpDHPartMessage = (bzrtpDHPartMessage_t *)malloc(sizeof(bzrtpDHPartMessage_t));
				memset(zrtpDHPartMessage, 0, sizeof(bzrtpDHPartMessage_t));
				 
				memcpy(zrtpDHPartMessage->H1, zrtpChannelContext->selfH[1], 32);
				 
				memcpy(zrtpDHPartMessage->rs1ID, zrtpContext->initiatorCachedSecretHash.rs1ID, 8);
				memcpy(zrtpDHPartMessage->rs2ID, zrtpContext->initiatorCachedSecretHash.rs2ID, 8);
				memcpy(zrtpDHPartMessage->auxsecretID, zrtpChannelContext->initiatorAuxsecretID, 8);
				memcpy(zrtpDHPartMessage->pbxsecretID, zrtpContext->initiatorCachedSecretHash.pbxsecretID, 8);
				
				 
				 
				switch (zrtpChannelContext->cipherAlgo) {
					case ZRTP_CIPHER_AES3:
					case ZRTP_CIPHER_2FS3:
						secretLength = 64;
						break;
					case ZRTP_CIPHER_AES2:
					case ZRTP_CIPHER_2FS2:
						secretLength = 48;
						break;
					case ZRTP_CIPHER_AES1:
					case ZRTP_CIPHER_2FS1:
					default:
						secretLength = 32;
						break;
				}

				switch (zrtpChannelContext->keyAgreementAlgo) {
					case ZRTP_KEYAGREEMENT_DH2k:
						bctoolbox_keyAgreementAlgo = BCTOOLBOX_DHM_2048;
						break;
					case ZRTP_KEYAGREEMENT_DH3k:
						bctoolbox_keyAgreementAlgo = BCTOOLBOX_DHM_3072;
						break;
					default:
						free(zrtpPacket);
						free(zrtpDHPartMessage);
						*exitCode = BZRTP_CREATE_ERROR_UNABLETOCREATECRYPTOCONTEXT;
						return NULL;
						break;
				}
				zrtpContext->DHMContext = bctoolbox_CreateDHMContext(bctoolbox_keyAgreementAlgo, secretLength);
				if (zrtpContext->DHMContext == NULL) {
					free(zrtpPacket);
					free(zrtpDHPartMessage);
					*exitCode = BZRTP_CREATE_ERROR_UNABLETOCREATECRYPTOCONTEXT;
					return NULL;
				}

				 
				bctoolbox_DHMCreatePublic(zrtpContext->DHMContext, (int (*)(void *, uint8_t *, size_t))bctoolbox_rng_get, zrtpContext->RNGContext);
				zrtpDHPartMessage->pv = (uint8_t *)malloc((zrtpChannelContext->keyAgreementLength)*sizeof(uint8_t));
				memcpy(zrtpDHPartMessage->pv, zrtpContext->DHMContext->self, zrtpChannelContext->keyAgreementLength);

				 
				zrtpPacket->messageData = zrtpDHPartMessage;
			}
			break;  

		case MSGTYPE_CONFIRM1:
		case MSGTYPE_CONFIRM2:
			{
				bzrtpConfirmMessage_t *zrtpConfirmMessage = (bzrtpConfirmMessage_t *)malloc(sizeof(bzrtpConfirmMessage_t));
				memset(zrtpConfirmMessage, 0, sizeof(bzrtpConfirmMessage_t));
				 
				memcpy(zrtpConfirmMessage->H0, zrtpChannelContext->selfH[0], 32);
				zrtpConfirmMessage->sig_len = 0;  
				zrtpConfirmMessage->cacheExpirationInterval = 0xFFFFFFFF;  
				zrtpConfirmMessage->E = 0;  
				zrtpConfirmMessage->V = zrtpContext->cachedSecret.previouslyVerifiedSas;
				zrtpConfirmMessage->A = 0;  
				zrtpConfirmMessage->D = 0;  

				 
				bctoolbox_rng_get(zrtpContext->RNGContext, zrtpConfirmMessage->CFBIV, 16);

				 
				zrtpPacket->messageData = zrtpConfirmMessage;
			}
			break;  

		case MSGTYPE_CONF2ACK :
			{
				 
			}
			break;  
		case MSGTYPE_PINGACK:
			{
				bzrtpPingMessage_t *pingMessage;
				bzrtpPingAckMessage_t *zrtpPingAckMessage;

				 
				bzrtpPacket_t *pingPacket = zrtpChannelContext->pingPacket;
				if (pingPacket == NULL) {
					*exitCode = BZRTP_CREATE_ERROR_INVALIDCONTEXT;
					return NULL;
				}
				pingMessage = (bzrtpPingMessage_t *)pingPacket->messageData;

				 
				zrtpPingAckMessage = (bzrtpPingAckMessage_t *)malloc(sizeof(bzrtpPingAckMessage_t));
				memset(zrtpPingAckMessage, 0, sizeof(bzrtpPingAckMessage_t));

				 
				memcpy(zrtpPingAckMessage->version,ZRTP_VERSION , 4);  
				memcpy(zrtpPingAckMessage->endpointHash, zrtpContext->selfZID, 8);  
				memcpy(zrtpPingAckMessage->endpointHashReceived, pingMessage->endpointHash, 8);
				zrtpPingAckMessage->SSRC = pingPacket->sourceIdentifier;

				 
				zrtpPacket->messageData = zrtpPingAckMessage;
			}  
			break;

		default:
			free(zrtpPacket);
			*exitCode = BZRTP_CREATE_ERROR_INVALIDMESSAGETYPE;
			return NULL;
			break;
	}

	zrtpPacket->sequenceNumber = 0;  
	zrtpPacket->messageType = messageType;
	zrtpPacket->sourceIdentifier = zrtpChannelContext->selfSSRC;
	zrtpPacket->messageLength = 0;  
	zrtpPacket->packetString = NULL;

	*exitCode=0;
	return zrtpPacket;
}

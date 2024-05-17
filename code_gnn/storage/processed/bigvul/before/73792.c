void test_parserComplete() {

	int retval;
	 
	bzrtpPacket_t *alice_Hello, *alice_HelloFromBob, *alice_HelloACK, *alice_HelloACKFromBob;
	 
	bzrtpPacket_t *bob_Hello, *bob_HelloFromAlice, *bob_HelloACK, *bob_HelloACKFromAlice;
	 
	bzrtpContext_t *contextAlice = bzrtp_createBzrtpContext(0x12345678);  
	bzrtpContext_t *contextBob = bzrtp_createBzrtpContext(0x87654321);  

	bzrtpHelloMessage_t *alice_HelloFromBob_message;
	bzrtpHelloMessage_t *bob_HelloFromAlice_message;
	bzrtpPacket_t *alice_selfDHPart;
	bzrtpPacket_t *bob_selfDHPart;
	bzrtpPacket_t *alice_Commit;
	bzrtpPacket_t *bob_Commit;
	bzrtpPacket_t *bob_CommitFromAlice;
	bzrtpPacket_t *alice_CommitFromBob;
	uint8_t tmpBuffer[8];
	bzrtpDHPartMessage_t *bob_DHPart1;
	bzrtpPacket_t *alice_DHPart1FromBob;
	bzrtpDHPartMessage_t *alice_DHPart1FromBob_message=NULL;
	bzrtpPacket_t *bob_DHPart2FromAlice;
	bzrtpDHPartMessage_t *bob_DHPart2FromAlice_message=NULL;
	uint16_t secretLength;
	uint16_t totalHashDataLength;
	uint8_t *dataToHash;
	uint16_t hashDataIndex = 0;
	uint8_t alice_totalHash[32];  
	uint8_t bob_totalHash[32];  
	uint8_t *s1=NULL;
	uint32_t s1Length=0;
	uint8_t *s2=NULL;
	uint32_t s2Length=0;
	uint8_t *s3=NULL;
	uint32_t s3Length=0;
	uint8_t alice_sasHash[32];
	uint8_t bob_sasHash[32];
	uint32_t sasValue;
	char sas[32];
	bzrtpPacket_t *bob_Confirm1;
	bzrtpPacket_t *alice_Confirm1FromBob;
	bzrtpConfirmMessage_t *alice_Confirm1FromBob_message=NULL;
	bzrtpPacket_t *alice_Confirm2;
	bzrtpPacket_t *bob_Confirm2FromAlice;
	bzrtpConfirmMessage_t *bob_Confirm2FromAlice_message=NULL;
	bzrtpPacket_t *bob_Conf2ACK;
	bzrtpPacket_t *alice_Conf2ACKFromBob;
	bzrtpPacket_t *alice_Confirm1;
	bzrtpPacket_t *bob_Confirm1FromAlice;
	bzrtpConfirmMessage_t *bob_Confirm1FromAlice_message=NULL;
	bzrtpPacket_t *bob_Confirm2;
	bzrtpPacket_t *alice_Confirm2FromBob;
	bzrtpConfirmMessage_t *alice_Confirm2FromBob_message=NULL;
	bzrtpPacket_t *alice_Conf2ACK;
	bzrtpPacket_t *bob_Conf2ACKFromAlice;
	bzrtpCallbacks_t cbs={0};

	 
	my_Context_t clientContextAlice;
	my_Context_t clientContextBob;
	strcpy(clientContextAlice.zidFilename, "./ZIDAlice.txt");
	strcpy(clientContextBob.zidFilename, "./ZIDBob.txt");

	 
	retval = bzrtp_setClientData(contextAlice, 0x12345678, (void *)&clientContextAlice);
	retval += bzrtp_setClientData(contextBob, 0x87654321, (void *)&clientContextBob);

	 
	cbs.bzrtp_loadCache=floadAlice;
	cbs.bzrtp_writeCache=fwriteAlice;

	bzrtp_setCallbacks(contextAlice, &cbs);

	cbs.bzrtp_loadCache=floadBob;
	cbs.bzrtp_writeCache=fwriteBob;
	bzrtp_setCallbacks(contextBob, &cbs);

	bzrtp_message ("Init the contexts\n");
	 
	bzrtp_initBzrtpContext(contextAlice);
	bzrtp_initBzrtpContext(contextBob);

	 
	alice_Hello = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[0], MSGTYPE_HELLO, &retval);
	if (bzrtp_packetBuild(contextAlice, contextAlice->channelContext[0], alice_Hello, contextAlice->channelContext[0]->selfSequenceNumber) ==0) {
		contextAlice->channelContext[0]->selfSequenceNumber++;
		contextAlice->channelContext[0]->selfPackets[HELLO_MESSAGE_STORE_ID] = alice_Hello;
	}
	bob_Hello = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_HELLO, &retval);
	if (bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_Hello, contextBob->channelContext[0]->selfSequenceNumber) ==0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
		contextBob->channelContext[0]->selfPackets[HELLO_MESSAGE_STORE_ID] = bob_Hello;
	}

	 
	alice_HelloFromBob = bzrtp_packetCheck(bob_Hello->packetString, bob_Hello->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_Hello->packetString, bob_Hello->messageLength+16, alice_HelloFromBob);
	bzrtp_message ("Alice parsing returns %x\n", retval);
	if (retval==0) {
		bzrtpHelloMessage_t *alice_HelloFromBob_message;
		int i;

		contextAlice->channelContext[0]->peerSequenceNumber = alice_HelloFromBob->sequenceNumber;
		 
		contextAlice->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID] = alice_HelloFromBob;

		 
		alice_HelloFromBob_message = (bzrtpHelloMessage_t *)alice_HelloFromBob->messageData;
		retval = crypoAlgoAgreement(contextAlice, contextAlice->channelContext[0], contextAlice->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageData);
		if (retval == 0) {
			bzrtp_message ("Alice selected algo %x\n", contextAlice->channelContext[0]->keyAgreementAlgo);
			memcpy(contextAlice->peerZID, alice_HelloFromBob_message->ZID, 12);
		}

		 
		for (i=0; i<alice_HelloFromBob_message->kc; i++) {
			if (alice_HelloFromBob_message->supportedKeyAgreement[i] == ZRTP_KEYAGREEMENT_Mult) {
				contextAlice->peerSupportMultiChannel = 1;
			}
		}
	}

	bob_HelloFromAlice = bzrtp_packetCheck(alice_Hello->packetString, alice_Hello->messageLength+16, contextBob->channelContext[0]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextBob, contextBob->channelContext[0], alice_Hello->packetString, alice_Hello->messageLength+16, bob_HelloFromAlice);
	bzrtp_message ("Bob parsing returns %x\n", retval);
	if (retval==0) {
		bzrtpHelloMessage_t *bob_HelloFromAlice_message;
		int i;

		contextBob->channelContext[0]->peerSequenceNumber = bob_HelloFromAlice->sequenceNumber;
		 
		contextBob->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID] = bob_HelloFromAlice;

		 
		bob_HelloFromAlice_message = (bzrtpHelloMessage_t *)bob_HelloFromAlice->messageData;
		retval = crypoAlgoAgreement(contextBob, contextBob->channelContext[0], contextBob->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageData);
		if (retval == 0) {
			bzrtp_message ("Bob selected algo %x\n", contextBob->channelContext[0]->keyAgreementAlgo);
			memcpy(contextBob->peerZID, bob_HelloFromAlice_message->ZID, 12);
		}

		 
		for (i=0; i<bob_HelloFromAlice_message->kc; i++) {
			if (bob_HelloFromAlice_message->supportedKeyAgreement[i] == ZRTP_KEYAGREEMENT_Mult) {
				contextBob->peerSupportMultiChannel = 1;
			}
		}
	}

	 
	alice_HelloFromBob_message = (bzrtpHelloMessage_t *)alice_HelloFromBob->messageData;
	memcpy(contextAlice->channelContext[0]->peerH[3], alice_HelloFromBob_message->H3, 32);
	bob_HelloFromAlice_message = (bzrtpHelloMessage_t *)bob_HelloFromAlice->messageData;
	memcpy(contextBob->channelContext[0]->peerH[3], bob_HelloFromAlice_message->H3, 32);

	 
	bzrtp_getPeerAssociatedSecretsHash(contextAlice, alice_HelloFromBob_message->ZID);
	bzrtp_getPeerAssociatedSecretsHash(contextBob, bob_HelloFromAlice_message->ZID);

	 
	if (contextAlice->cachedSecret.rs1!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.rs1, contextAlice->cachedSecret.rs1Length, (uint8_t *)"Initiator", 9, 8, contextAlice->initiatorCachedSecretHash.rs1ID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->initiatorCachedSecretHash.rs1ID, 8);
	}

	if (contextAlice->cachedSecret.rs2!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.rs2, contextAlice->cachedSecret.rs2Length, (uint8_t *)"Initiator", 9, 8, contextAlice->initiatorCachedSecretHash.rs2ID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->initiatorCachedSecretHash.rs2ID, 8);
	}

	if (contextAlice->cachedSecret.auxsecret!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.auxsecret, contextAlice->cachedSecret.auxsecretLength, contextAlice->channelContext[0]->selfH[3], 32, 8, contextAlice->channelContext[0]->initiatorAuxsecretID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->channelContext[0]->initiatorAuxsecretID, 8);
	}

	if (contextAlice->cachedSecret.pbxsecret!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.pbxsecret, contextAlice->cachedSecret.pbxsecretLength, (uint8_t *)"Initiator", 9, 8, contextAlice->initiatorCachedSecretHash.pbxsecretID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->initiatorCachedSecretHash.pbxsecretID, 8);
	}

	if (contextAlice->cachedSecret.rs1!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.rs1, contextAlice->cachedSecret.rs1Length, (uint8_t *)"Responder", 9, 8, contextAlice->responderCachedSecretHash.rs1ID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->responderCachedSecretHash.rs1ID, 8);
	}

	if (contextAlice->cachedSecret.rs2!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.rs2, contextAlice->cachedSecret.rs2Length, (uint8_t *)"Responder", 9, 8, contextAlice->responderCachedSecretHash.rs2ID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->responderCachedSecretHash.rs2ID, 8);
	}

	if (contextAlice->cachedSecret.auxsecret!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.auxsecret, contextAlice->cachedSecret.auxsecretLength, contextAlice->channelContext[0]->peerH[3], 32, 8, contextAlice->channelContext[0]->responderAuxsecretID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->channelContext[0]->responderAuxsecretID, 8);
	}

	if (contextAlice->cachedSecret.pbxsecret!=NULL) {
		contextAlice->channelContext[0]->hmacFunction(contextAlice->cachedSecret.pbxsecret, contextAlice->cachedSecret.pbxsecretLength, (uint8_t *)"Responder", 9, 8, contextAlice->responderCachedSecretHash.pbxsecretID);
	} else {  
		bctoolbox_rng_get(contextAlice->RNGContext, contextAlice->responderCachedSecretHash.pbxsecretID, 8);
	}


	 
	if (contextBob->cachedSecret.rs1!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.rs1, contextBob->cachedSecret.rs1Length, (uint8_t *)"Initiator", 9, 8, contextBob->initiatorCachedSecretHash.rs1ID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->initiatorCachedSecretHash.rs1ID, 8);
	}

	if (contextBob->cachedSecret.rs2!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.rs2, contextBob->cachedSecret.rs2Length, (uint8_t *)"Initiator", 9, 8, contextBob->initiatorCachedSecretHash.rs2ID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->initiatorCachedSecretHash.rs2ID, 8);
	}

	if (contextBob->cachedSecret.auxsecret!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.auxsecret, contextBob->cachedSecret.auxsecretLength, contextBob->channelContext[0]->selfH[3], 32, 8, contextBob->channelContext[0]->initiatorAuxsecretID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->channelContext[0]->initiatorAuxsecretID, 8);
	}

	if (contextBob->cachedSecret.pbxsecret!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.pbxsecret, contextBob->cachedSecret.pbxsecretLength, (uint8_t *)"Initiator", 9, 8, contextBob->initiatorCachedSecretHash.pbxsecretID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->initiatorCachedSecretHash.pbxsecretID, 8);
	}

	if (contextBob->cachedSecret.rs1!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.rs1, contextBob->cachedSecret.rs1Length, (uint8_t *)"Responder", 9, 8, contextBob->responderCachedSecretHash.rs1ID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->responderCachedSecretHash.rs1ID, 8);
	}

	if (contextBob->cachedSecret.rs2!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.rs2, contextBob->cachedSecret.rs2Length, (uint8_t *)"Responder", 9, 8, contextBob->responderCachedSecretHash.rs2ID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->responderCachedSecretHash.rs2ID, 8);
	}

	if (contextBob->cachedSecret.auxsecret!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.auxsecret, contextBob->cachedSecret.auxsecretLength, contextBob->channelContext[0]->peerH[3], 32, 8, contextBob->channelContext[0]->responderAuxsecretID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->channelContext[0]->responderAuxsecretID, 8);
	}

	if (contextBob->cachedSecret.pbxsecret!=NULL) {
		contextBob->channelContext[0]->hmacFunction(contextBob->cachedSecret.pbxsecret, contextBob->cachedSecret.pbxsecretLength, (uint8_t *)"Responder", 9, 8, contextBob->responderCachedSecretHash.pbxsecretID);
	} else {  
		bctoolbox_rng_get(contextBob->RNGContext, contextBob->responderCachedSecretHash.pbxsecretID, 8);
	}

	 
	bzrtp_message ("\nAlice original Packet is \n");
	packetDump(alice_Hello, 1);
	bzrtp_message ("\nBob's parsed Alice Packet is \n");
	packetDump(bob_HelloFromAlice, 0);

	 
	alice_selfDHPart = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[0], MSGTYPE_DHPART2, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[0], alice_selfDHPart, 0);  
	if (retval == 0) {  
		contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID] = alice_selfDHPart;
	} else {
		bzrtp_message ("Alice building DHPart packet returns %x\n", retval);
	}
	bob_selfDHPart = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_DHPART2, &retval);
	retval +=bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_selfDHPart, 0);  
	if (retval == 0) {  
		contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID] = bob_selfDHPart;
	} else {
		bzrtp_message ("Bob building DHPart packet returns %x\n", retval);
	}
	bzrtp_message("Alice DHPart packet:\n");
	packetDump(alice_selfDHPart,0);
	bzrtp_message("Bob DHPart packet:\n");
	packetDump(bob_selfDHPart,0);

	 
	alice_HelloACK = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[0], MSGTYPE_HELLOACK, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[0], alice_HelloACK, contextAlice->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextAlice->channelContext[0]->selfSequenceNumber++;
	} else {
		bzrtp_message("Alice building HelloACK return %x\n", retval);
	}

	bob_HelloACK = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_HELLOACK, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_HelloACK, contextBob->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
	} else {
		bzrtp_message("Bob building HelloACK return %x\n", retval);
	}

	 
	alice_HelloACKFromBob = bzrtp_packetCheck(bob_HelloACK->packetString, bob_HelloACK->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_HelloACK->packetString, bob_HelloACK->messageLength+16, alice_HelloACKFromBob);
	bzrtp_message ("Alice parsing Hello ACK returns %x\n", retval);
	if (retval==0) {
		contextAlice->channelContext[0]->peerSequenceNumber = alice_HelloACKFromBob->sequenceNumber;
	}

	bob_HelloACKFromAlice = bzrtp_packetCheck(alice_HelloACK->packetString, alice_HelloACK->messageLength+16, contextBob->channelContext[0]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextBob, contextBob->channelContext[0], alice_HelloACK->packetString, alice_HelloACK->messageLength+16, bob_HelloACKFromAlice);
	bzrtp_message ("Bob parsing Hello ACK returns %x\n", retval);
	if (retval==0) {
		contextBob->channelContext[0]->peerSequenceNumber = bob_HelloACKFromAlice->sequenceNumber;
	}
	bzrtp_freeZrtpPacket(alice_HelloACK);
	bzrtp_freeZrtpPacket(bob_HelloACK);
	bzrtp_freeZrtpPacket(alice_HelloACKFromBob);
	bzrtp_freeZrtpPacket(bob_HelloACKFromAlice);


	 
	alice_Commit = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[0], MSGTYPE_COMMIT, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[0], alice_Commit, contextAlice->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextAlice->channelContext[0]->selfSequenceNumber++;
		contextAlice->channelContext[0]->selfPackets[COMMIT_MESSAGE_STORE_ID] = alice_Commit;
	}
	bzrtp_message("Alice building Commit return %x\n", retval);

	bob_Commit = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_COMMIT, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_Commit, contextBob->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
		contextBob->channelContext[0]->selfPackets[COMMIT_MESSAGE_STORE_ID] = bob_Commit;
	}
	bzrtp_message("Bob building Commit return %x\n", retval);


	 
	bob_CommitFromAlice = bzrtp_packetCheck(alice_Commit->packetString, alice_Commit->messageLength+16, contextBob->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextBob, contextBob->channelContext[0], alice_Commit->packetString, alice_Commit->messageLength+16, bob_CommitFromAlice);
	bzrtp_message ("Bob parsing Commit returns %x\n", retval);
	if (retval==0) {
		 
		bzrtpCommitMessage_t *bob_CommitFromAlice_message = (bzrtpCommitMessage_t *)bob_CommitFromAlice->messageData;
		contextBob->channelContext[0]->peerSequenceNumber = bob_CommitFromAlice->sequenceNumber;
		memcpy(contextBob->channelContext[0]->peerH[2], bob_CommitFromAlice_message->H2, 32);
		contextBob->channelContext[0]->peerPackets[COMMIT_MESSAGE_STORE_ID] = bob_CommitFromAlice;
	}
	packetDump(bob_CommitFromAlice, 0);

	alice_CommitFromBob = bzrtp_packetCheck(bob_Commit->packetString, bob_Commit->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_Commit->packetString, bob_Commit->messageLength+16, alice_CommitFromBob);
	bzrtp_message ("Alice parsing Commit returns %x\n", retval);
	if (retval==0) {
		 
		contextAlice->channelContext[0]->peerSequenceNumber = alice_CommitFromBob->sequenceNumber;
		 
		 
	}
	packetDump(alice_CommitFromBob, 0);
	bzrtp_freeZrtpPacket(alice_CommitFromBob);

	 
	 
	 
	contextBob->channelContext[0]->role = RESPONDER;

	 
	 

	 
	 
	memcpy(tmpBuffer, contextBob->channelContext[0]->initiatorAuxsecretID, 8);
	memcpy(contextBob->channelContext[0]->initiatorAuxsecretID, contextBob->channelContext[0]->responderAuxsecretID, 8);
	memcpy(contextBob->channelContext[0]->responderAuxsecretID, tmpBuffer, 8);

	contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageType = MSGTYPE_DHPART1;  
	bob_DHPart1 = (bzrtpDHPartMessage_t *)contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageData;
	 
	memcpy(bob_DHPart1->rs1ID, contextBob->responderCachedSecretHash.rs1ID, 8);
	memcpy(bob_DHPart1->rs2ID, contextBob->responderCachedSecretHash.rs2ID, 8);
	memcpy(bob_DHPart1->auxsecretID, contextBob->channelContext[0]->responderAuxsecretID, 8);
	memcpy(bob_DHPart1->pbxsecretID, contextBob->responderCachedSecretHash.pbxsecretID, 8);

	retval +=bzrtp_packetBuild(contextBob, contextBob->channelContext[0], contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID],contextBob->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
	}
	bzrtp_message("Bob building DHPart1 return %x\n", retval);


	 
	alice_DHPart1FromBob = bzrtp_packetCheck(contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString, contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString, contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength+16, alice_DHPart1FromBob);
	bzrtp_message ("Alice parsing DHPart1 returns %x\n", retval);
	if (retval==0) {
		 
		contextAlice->channelContext[0]->peerSequenceNumber = alice_DHPart1FromBob->sequenceNumber;
		alice_DHPart1FromBob_message = (bzrtpDHPartMessage_t *)alice_DHPart1FromBob->messageData;
		memcpy(contextAlice->channelContext[0]->peerH[1], alice_DHPart1FromBob_message->H1, 32);
		contextAlice->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID] = alice_DHPart1FromBob;
	}
	packetDump(alice_DHPart1FromBob, 1);

	 
	if (contextAlice->cachedSecret.rs1!=NULL) {
		if (memcmp(contextAlice->responderCachedSecretHash.rs1ID, alice_DHPart1FromBob_message->rs1ID,8) != 0) {
			bzrtp_message ("Alice found that requested shared secret rs1 ID differs!\n");
		} else {
			bzrtp_message("Alice validate rs1ID from bob DHPart1\n");
		}
	}
	if (contextAlice->cachedSecret.rs2!=NULL) {
		if (memcmp(contextAlice->responderCachedSecretHash.rs2ID, alice_DHPart1FromBob_message->rs2ID,8) != 0) {
			bzrtp_message ("Alice found that requested shared secret rs2 ID differs!\n");
		} else {
			bzrtp_message("Alice validate rs2ID from bob DHPart1\n");
		}
	}
	if (contextAlice->cachedSecret.auxsecret!=NULL) {
		if (memcmp(contextAlice->channelContext[0]->responderAuxsecretID, alice_DHPart1FromBob_message->auxsecretID,8) != 0) {
			bzrtp_message ("Alice found that requested shared secret aux secret ID differs!\n");
		} else {
			bzrtp_message("Alice validate aux secret ID from bob DHPart1\n");
		}
	}
	if (contextAlice->cachedSecret.pbxsecret!=NULL) {
		if (memcmp(contextAlice->responderCachedSecretHash.pbxsecretID, alice_DHPart1FromBob_message->pbxsecretID,8) != 0) {
			bzrtp_message ("Alice found that requested shared secret pbx secret ID differs!\n");
		} else {
			bzrtp_message("Alice validate pbxsecretID from bob DHPart1\n");
		}
	}

	 
	 
	contextAlice->DHMContext->peer = (uint8_t *)malloc(contextAlice->channelContext[0]->keyAgreementLength*sizeof(uint8_t));
	memcpy (contextAlice->DHMContext->peer, alice_DHPart1FromBob_message->pv, contextAlice->channelContext[0]->keyAgreementLength);
	bctoolbox_DHMComputeSecret(contextAlice->DHMContext, (int (*)(void *, uint8_t *, size_t))bctoolbox_rng_get, (void *)contextAlice->RNGContext);

	 
	bzrtp_packetUpdateSequenceNumber(contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID], contextAlice->channelContext[0]->selfSequenceNumber);
	contextAlice->channelContext[0]->selfSequenceNumber++;

	 
	bob_DHPart2FromAlice = bzrtp_packetCheck(contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString, contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength+16, contextBob->channelContext[0]->peerSequenceNumber, &retval);
	bzrtp_message ("Bob checking DHPart2 returns %x\n", retval);
	retval += bzrtp_packetParser(contextBob, contextBob->channelContext[0], contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString, contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength+16, bob_DHPart2FromAlice);
	bzrtp_message ("Bob parsing DHPart2 returns %x\n", retval);
	if (retval==0) {
		 
		contextBob->channelContext[0]->peerSequenceNumber = bob_DHPart2FromAlice->sequenceNumber;
		bob_DHPart2FromAlice_message = (bzrtpDHPartMessage_t *)bob_DHPart2FromAlice->messageData;
		memcpy(contextBob->channelContext[0]->peerH[1], bob_DHPart2FromAlice_message->H1, 32);
		contextBob->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID] = bob_DHPart2FromAlice;
	}
	packetDump(bob_DHPart2FromAlice, 0);

	 
	if (contextBob->cachedSecret.rs1!=NULL) {
		if (memcmp(contextBob->initiatorCachedSecretHash.rs1ID, bob_DHPart2FromAlice_message->rs1ID,8) != 0) {
			bzrtp_message ("Bob found that requested shared secret rs1 ID differs!\n");
		} else {
			bzrtp_message("Bob validate rs1ID from Alice DHPart2\n");
		}
	}
	if (contextBob->cachedSecret.rs2!=NULL) {
		if (memcmp(contextBob->initiatorCachedSecretHash.rs2ID, bob_DHPart2FromAlice_message->rs2ID,8) != 0) {
			bzrtp_message ("Bob found that requested shared secret rs2 ID differs!\n");
		} else {
			bzrtp_message("Bob validate rs2ID from Alice DHPart2\n");
		}
	}
	if (contextBob->cachedSecret.auxsecret!=NULL) {
		if (memcmp(contextBob->channelContext[0]->initiatorAuxsecretID, bob_DHPart2FromAlice_message->auxsecretID,8) != 0) {
			bzrtp_message ("Bob found that requested shared secret aux secret ID differs!\n");
		} else {
			bzrtp_message("Bob validate aux secret ID from Alice DHPart2\n");
		}
	}
	if (contextBob->cachedSecret.pbxsecret!=NULL) {
		if (memcmp(contextBob->initiatorCachedSecretHash.pbxsecretID, bob_DHPart2FromAlice_message->pbxsecretID,8) != 0) {
			bzrtp_message ("Bob found that requested shared secret pbx secret ID differs!\n");
		} else {
			bzrtp_message("Bob validate pbxsecretID from Alice DHPart2\n");
		}
	}

	 
	 
	contextBob->DHMContext->peer = (uint8_t *)malloc(contextBob->channelContext[0]->keyAgreementLength*sizeof(uint8_t));
	memcpy (contextBob->DHMContext->peer, bob_DHPart2FromAlice_message->pv, contextBob->channelContext[0]->keyAgreementLength);
	bctoolbox_DHMComputeSecret(contextBob->DHMContext, (int (*)(void *, uint8_t *, size_t))bctoolbox_rng_get, (void *)contextAlice->RNGContext);


	 
	secretLength = bob_DHPart2FromAlice->messageLength-84;  
	if (memcmp(contextBob->DHMContext->key, contextAlice->DHMContext->key, secretLength)==0) {
		bzrtp_message("Secret Key correctly exchanged \n");
		CU_PASS("Secret Key exchange OK");
	} else {
		CU_FAIL("Secret Key exchange failed");
		bzrtp_message("ERROR : secretKey exchange failed!!\n");
	}

	 
	totalHashDataLength = bob_Hello->messageLength + alice_Commit->messageLength + contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength + alice_selfDHPart->messageLength;
	dataToHash = (uint8_t *)malloc(totalHashDataLength*sizeof(uint8_t));
	 
	memcpy(dataToHash, contextAlice->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextAlice->channelContext[0]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextAlice->channelContext[0]->selfPackets[COMMIT_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[0]->selfPackets[COMMIT_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextAlice->channelContext[0]->selfPackets[COMMIT_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextAlice->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextAlice->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength);

	contextAlice->channelContext[0]->hashFunction(dataToHash, totalHashDataLength, 32, alice_totalHash);

	 
	hashDataIndex = 0;
	memcpy(dataToHash, contextBob->channelContext[0]->selfPackets[HELLO_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[0]->selfPackets[HELLO_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextBob->channelContext[0]->selfPackets[HELLO_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextBob->channelContext[0]->peerPackets[COMMIT_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[0]->peerPackets[COMMIT_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextBob->channelContext[0]->peerPackets[COMMIT_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextBob->channelContext[0]->selfPackets[DHPART_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextBob->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[0]->peerPackets[DHPART_MESSAGE_STORE_ID]->messageLength);

	contextBob->channelContext[0]->hashFunction(dataToHash, totalHashDataLength, 32, bob_totalHash);
	if (memcmp(bob_totalHash, alice_totalHash, 32) == 0) {
		bzrtp_message("Got the same total hash\n");
		CU_PASS("Total Hash match");
	} else {
		bzrtp_message("AARGG!! total hash mismatch");
		CU_FAIL("Total Hash mismatch");
	}

	 
	free(dataToHash);
	contextAlice->channelContext[0]->KDFContextLength = 24+32; 
	contextAlice->channelContext[0]->KDFContext = (uint8_t *)malloc(contextAlice->channelContext[0]->KDFContextLength*sizeof(uint8_t));
	memcpy(contextAlice->channelContext[0]->KDFContext, contextAlice->selfZID, 12);  
	memcpy(contextAlice->channelContext[0]->KDFContext+12, contextAlice->peerZID, 12);  
	memcpy(contextAlice->channelContext[0]->KDFContext+24, alice_totalHash, 32);  

	 
	if (contextAlice->cachedSecret.rs1 != NULL) {  
		s1 = contextAlice->cachedSecret.rs1;
		s1Length = contextAlice->cachedSecret.rs1Length;
	} else if (contextAlice->cachedSecret.rs2 != NULL) {  
		s1 = contextAlice->cachedSecret.rs2;
		s1Length = contextAlice->cachedSecret.rs2Length;
	}

	 
	s2 = contextAlice->cachedSecret.auxsecret;  
	s2Length = contextAlice->cachedSecret.auxsecretLength;  

	 
	s3 = contextAlice->cachedSecret.pbxsecret;  
	s3Length = contextAlice->cachedSecret.pbxsecretLength;  

	totalHashDataLength = 4+secretLength+13  + 12 + 12 + 32 + 4 +s1Length + 4 +s2Length + 4 + s3Length;  

	dataToHash = (uint8_t *)malloc(totalHashDataLength*sizeof(uint8_t));
	dataToHash[0] = 0x00;
	dataToHash[1] = 0x00;
	dataToHash[2] = 0x00;
	dataToHash[3] = 0x01;
	hashDataIndex = 4;

	memcpy(dataToHash+hashDataIndex, contextAlice->DHMContext->key, secretLength);
	hashDataIndex += secretLength;
	memcpy(dataToHash+hashDataIndex, "ZRTP-HMAC-KDF", 13);
	hashDataIndex += 13;
	memcpy(dataToHash+hashDataIndex, contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength);
	hashDataIndex += 56;

	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s1Length&0xFF);
	if (s1!=NULL) {
		memcpy(dataToHash+hashDataIndex, s1, s1Length);
		hashDataIndex += s1Length;
	}

	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s2Length&0xFF);
	if (s2!=NULL) {
		memcpy(dataToHash+hashDataIndex, s2, s2Length);
		hashDataIndex += s2Length;
	}

	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s3Length&0xFF);
	if (s3!=NULL) {
		memcpy(dataToHash+hashDataIndex, s3, s3Length);
		hashDataIndex += s3Length;
	}

	contextAlice->channelContext[0]->s0 = (uint8_t *)malloc(32*sizeof(uint8_t));
	contextAlice->channelContext[0]->hashFunction(dataToHash, totalHashDataLength, 32, contextAlice->channelContext[0]->s0);

	 
	if (contextAlice->cachedSecret.rs1!=NULL) {
		bzrtp_DestroyKey(contextAlice->cachedSecret.rs1, contextAlice->cachedSecret.rs1Length, contextAlice->RNGContext);
		free(contextAlice->cachedSecret.rs1);
		contextAlice->cachedSecret.rs1 = NULL;
	}
	if (contextAlice->cachedSecret.rs2!=NULL) {
		bzrtp_DestroyKey(contextAlice->cachedSecret.rs2, contextAlice->cachedSecret.rs2Length, contextAlice->RNGContext);
		free(contextAlice->cachedSecret.rs2);
		contextAlice->cachedSecret.rs2 = NULL;
	}
	if (contextAlice->cachedSecret.auxsecret!=NULL) {
		bzrtp_DestroyKey(contextAlice->cachedSecret.auxsecret, contextAlice->cachedSecret.auxsecretLength, contextAlice->RNGContext);
		free(contextAlice->cachedSecret.auxsecret);
		contextAlice->cachedSecret.auxsecret = NULL;
	}
	if (contextAlice->cachedSecret.pbxsecret!=NULL) {
		bzrtp_DestroyKey(contextAlice->cachedSecret.pbxsecret, contextAlice->cachedSecret.pbxsecretLength, contextAlice->RNGContext);
		free(contextAlice->cachedSecret.pbxsecret);
		contextAlice->cachedSecret.pbxsecret = NULL;
	}

	 
	 
	s1=NULL;
	s2=NULL;
	s3=NULL;
	contextBob->channelContext[0]->KDFContextLength = 24+32; 
	contextBob->channelContext[0]->KDFContext = (uint8_t *)malloc(contextBob->channelContext[0]->KDFContextLength*sizeof(uint8_t));
	memcpy(contextBob->channelContext[0]->KDFContext, contextBob->peerZID, 12);  
	memcpy(contextBob->channelContext[0]->KDFContext+12, contextBob->selfZID, 12);  
	memcpy(contextBob->channelContext[0]->KDFContext+24, bob_totalHash, 32);  

	if (contextBob->cachedSecret.rs1 != NULL) {  
		s1 = contextBob->cachedSecret.rs1;
		s1Length = contextBob->cachedSecret.rs1Length;
	} else if (contextBob->cachedSecret.rs2 != NULL) {  
		s1 = contextBob->cachedSecret.rs2;
		s1Length = contextBob->cachedSecret.rs2Length;
	}

	 
	s2 = contextBob->cachedSecret.auxsecret;  
	s2Length = contextBob->cachedSecret.auxsecretLength;  

	 
	s3 = contextBob->cachedSecret.pbxsecret;  
	s3Length = contextBob->cachedSecret.pbxsecretLength;  

	free(dataToHash);
	dataToHash = (uint8_t *)malloc(totalHashDataLength*sizeof(uint8_t));
	dataToHash[0] = 0x00;
	dataToHash[1] = 0x00;
	dataToHash[2] = 0x00;
	dataToHash[3] = 0x01;
	hashDataIndex = 4;

	memcpy(dataToHash+hashDataIndex, contextBob->DHMContext->key, secretLength);
	hashDataIndex += secretLength;
	memcpy(dataToHash+hashDataIndex, "ZRTP-HMAC-KDF", 13);
	hashDataIndex += 13;
	memcpy(dataToHash+hashDataIndex, contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength);
	hashDataIndex += 56;

	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s1Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s1Length&0xFF);
	if (s1!=NULL) {
		memcpy(dataToHash+hashDataIndex, s1, s1Length);
		hashDataIndex += s1Length;
	}

	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s2Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s2Length&0xFF);
	if (s2!=NULL) {
		memcpy(dataToHash+hashDataIndex, s2, s2Length);
		hashDataIndex += s2Length;
	}

	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>24)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>16)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)((s3Length>>8)&0xFF);
	dataToHash[hashDataIndex++] = (uint8_t)(s3Length&0xFF);
	if (s3!=NULL) {
		memcpy(dataToHash+hashDataIndex, s3, s3Length);
		hashDataIndex += s3Length;
	}

	contextBob->channelContext[0]->s0 = (uint8_t *)malloc(32*sizeof(uint8_t));
	contextBob->channelContext[0]->hashFunction(dataToHash, totalHashDataLength, 32, contextBob->channelContext[0]->s0);

	free(dataToHash);

	 
	if (contextBob->cachedSecret.rs1!=NULL) {
		bzrtp_DestroyKey(contextBob->cachedSecret.rs1, contextBob->cachedSecret.rs1Length, contextBob->RNGContext);
		free(contextBob->cachedSecret.rs1);
		contextBob->cachedSecret.rs1 = NULL;
	}
	if (contextBob->cachedSecret.rs2!=NULL) {
		bzrtp_DestroyKey(contextBob->cachedSecret.rs2, contextBob->cachedSecret.rs2Length, contextBob->RNGContext);
		free(contextBob->cachedSecret.rs2);
		contextBob->cachedSecret.rs2 = NULL;
	}
	if (contextBob->cachedSecret.auxsecret!=NULL) {
		bzrtp_DestroyKey(contextBob->cachedSecret.auxsecret, contextBob->cachedSecret.auxsecretLength, contextBob->RNGContext);
		free(contextBob->cachedSecret.auxsecret);
		contextBob->cachedSecret.auxsecret = NULL;
	}
	if (contextBob->cachedSecret.pbxsecret!=NULL) {
		bzrtp_DestroyKey(contextBob->cachedSecret.pbxsecret, contextBob->cachedSecret.pbxsecretLength, contextBob->RNGContext);
		free(contextBob->cachedSecret.pbxsecret);
		contextBob->cachedSecret.pbxsecret = NULL;
	}


	 
	if (memcmp(contextBob->channelContext[0]->s0, contextAlice->channelContext[0]->s0, 32)==0) {
		bzrtp_message("Got the same s0\n");
		CU_PASS("s0 match");
	} else {
		bzrtp_message("ERROR s0 differs\n");
		CU_PASS("s0 mismatch");
	}

	 
	contextAlice->ZRTPSessLength=32;  
	contextAlice->ZRTPSess = (uint8_t *)malloc(contextAlice->ZRTPSessLength*sizeof(uint8_t));
	retval = bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength,
		(uint8_t *)"ZRTP Session Key", 16,
		contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength,  
		contextAlice->channelContext[0]->hashLength,
		(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction,
		contextAlice->ZRTPSess);

	contextBob->ZRTPSessLength=32;  
	contextBob->ZRTPSess = (uint8_t *)malloc(contextBob->ZRTPSessLength*sizeof(uint8_t));
	retval = bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength,
		(uint8_t *)"ZRTP Session Key", 16,
		contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength,  
		contextBob->channelContext[0]->hashLength,
		(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction,
		contextBob->ZRTPSess);

	 
	if (memcmp(contextBob->ZRTPSess, contextAlice->ZRTPSess, 32)==0) {
		bzrtp_message("Got the same ZRTPSess\n");
		CU_PASS("ZRTPSess match");
	} else {
		bzrtp_message("ERROR ZRTPSess differs\n");
		CU_PASS("ZRTPSess mismatch");
	}


	 
	retval = bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength,
			(uint8_t *)"SAS", 3,
			contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength,  
			256/8,  
			(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction,
			alice_sasHash);

	retval = bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength,
			(uint8_t *)"SAS", 3,
			contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength,  
			256/8,  
			(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction,
			bob_sasHash);

	 
	if (memcmp(alice_sasHash, bob_sasHash, 32)==0) {
		bzrtp_message("Got the same SAS Hash\n");
		CU_PASS("SAS Hash match");
	} else {
		bzrtp_message("ERROR SAS Hash differs\n");
		CU_PASS("SAS Hash mismatch");
	}

	 
	sasValue = ((uint32_t)alice_sasHash[0]<<24) | ((uint32_t)alice_sasHash[1]<<16) | ((uint32_t)alice_sasHash[2]<<8) | ((uint32_t)(alice_sasHash[3]));
	contextAlice->channelContext[0]->sasFunction(sasValue, sas, 5);

	bzrtp_message("Alice SAS is %.4s\n", sas);

	sasValue = ((uint32_t)bob_sasHash[0]<<24) | ((uint32_t)bob_sasHash[1]<<16) | ((uint32_t)bob_sasHash[2]<<8) | ((uint32_t)(bob_sasHash[3]));
	contextBob->channelContext[0]->sasFunction(sasValue, sas, 5);

	bzrtp_message("Bob SAS is %.4s\n", sas);


	 
	contextAlice->channelContext[0]->mackeyi = (uint8_t *)malloc(contextAlice->channelContext[0]->hashLength*(sizeof(uint8_t)));
	contextAlice->channelContext[0]->mackeyr = (uint8_t *)malloc(contextAlice->channelContext[0]->hashLength*(sizeof(uint8_t)));
	contextAlice->channelContext[0]->zrtpkeyi = (uint8_t *)malloc(contextAlice->channelContext[0]->cipherKeyLength*(sizeof(uint8_t)));
	contextAlice->channelContext[0]->zrtpkeyr = (uint8_t *)malloc(contextAlice->channelContext[0]->cipherKeyLength*(sizeof(uint8_t)));
	contextBob->channelContext[0]->mackeyi = (uint8_t *)malloc(contextBob->channelContext[0]->hashLength*(sizeof(uint8_t)));
	contextBob->channelContext[0]->mackeyr = (uint8_t *)malloc(contextBob->channelContext[0]->hashLength*(sizeof(uint8_t)));
	contextBob->channelContext[0]->zrtpkeyi = (uint8_t *)malloc(contextBob->channelContext[0]->cipherKeyLength*(sizeof(uint8_t)));
	contextBob->channelContext[0]->zrtpkeyr = (uint8_t *)malloc(contextBob->channelContext[0]->cipherKeyLength*(sizeof(uint8_t)));

	 
	retval = bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength, (uint8_t *)"Initiator HMAC key", 18, contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength, contextAlice->channelContext[0]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction, contextAlice->channelContext[0]->mackeyi);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength, (uint8_t *)"Responder HMAC key", 18, contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength, contextAlice->channelContext[0]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction, contextAlice->channelContext[0]->mackeyr);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength, (uint8_t *)"Initiator ZRTP key", 18, contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength, contextAlice->channelContext[0]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction, contextAlice->channelContext[0]->zrtpkeyi);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[0]->s0, contextAlice->channelContext[0]->hashLength, (uint8_t *)"Responder ZRTP key", 18, contextAlice->channelContext[0]->KDFContext, contextAlice->channelContext[0]->KDFContextLength, contextAlice->channelContext[0]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[0]->hmacFunction, contextAlice->channelContext[0]->zrtpkeyr);

	 
	retval = bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength, (uint8_t *)"Initiator HMAC key", 18, contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength, contextBob->channelContext[0]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction, contextBob->channelContext[0]->mackeyi);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength, (uint8_t *)"Responder HMAC key", 18, contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength, contextBob->channelContext[0]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction, contextBob->channelContext[0]->mackeyr);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength, (uint8_t *)"Initiator ZRTP key", 18, contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength, contextBob->channelContext[0]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction, contextBob->channelContext[0]->zrtpkeyi);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[0]->s0, contextBob->channelContext[0]->hashLength, (uint8_t *)"Responder ZRTP key", 18, contextBob->channelContext[0]->KDFContext, contextBob->channelContext[0]->KDFContextLength, contextBob->channelContext[0]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[0]->hmacFunction, contextBob->channelContext[0]->zrtpkeyr);


	 
	if ((memcmp(contextAlice->channelContext[0]->mackeyi, contextBob->channelContext[0]->mackeyi, contextAlice->channelContext[0]->hashLength)==0) && (memcmp(contextAlice->channelContext[0]->mackeyr, contextBob->channelContext[0]->mackeyr, contextAlice->channelContext[0]->hashLength)==0) && (memcmp(contextAlice->channelContext[0]->zrtpkeyi, contextBob->channelContext[0]->zrtpkeyi, contextAlice->channelContext[0]->cipherKeyLength)==0) && (memcmp(contextAlice->channelContext[0]->zrtpkeyr, contextBob->channelContext[0]->zrtpkeyr, contextAlice->channelContext[0]->cipherKeyLength)==0)) {
		bzrtp_message("Got the same keys\n");
		CU_PASS("keys match");
	} else {
		bzrtp_message("ERROR keys differ\n");
		CU_PASS("Keys mismatch");
	}

	 
	bob_Confirm1 = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_CONFIRM1, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_Confirm1, contextBob->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
	}
	bzrtp_message("Bob building Confirm1 return %x\n", retval);

	alice_Confirm1FromBob = bzrtp_packetCheck(bob_Confirm1->packetString, bob_Confirm1->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_Confirm1->packetString, bob_Confirm1->messageLength+16, alice_Confirm1FromBob);
	bzrtp_message ("Alice parsing confirm1 returns %x\n", retval);
	if (retval==0) {
		 
		contextAlice->channelContext[0]->peerSequenceNumber = alice_Confirm1FromBob->sequenceNumber;
		alice_Confirm1FromBob_message = (bzrtpConfirmMessage_t *)alice_Confirm1FromBob->messageData;
		memcpy(contextAlice->channelContext[0]->peerH[0], alice_Confirm1FromBob_message->H0, 32);
	}

	packetDump(bob_Confirm1,1);
	packetDump(alice_Confirm1FromBob,0);

	bzrtp_freeZrtpPacket(alice_Confirm1FromBob);
	bzrtp_freeZrtpPacket(bob_Confirm1);

	 
	alice_Confirm2 = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[0], MSGTYPE_CONFIRM2, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[0], alice_Confirm2, contextAlice->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextAlice->channelContext[0]->selfSequenceNumber++;
	}
	bzrtp_message("Alice building Confirm2 return %x\n", retval);

	bob_Confirm2FromAlice = bzrtp_packetCheck(alice_Confirm2->packetString, alice_Confirm2->messageLength+16, contextBob->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextBob, contextBob->channelContext[0], alice_Confirm2->packetString, alice_Confirm2->messageLength+16, bob_Confirm2FromAlice);
	bzrtp_message ("Bob parsing confirm2 returns %x\n", retval);
	if (retval==0) {
		 
		contextBob->channelContext[0]->peerSequenceNumber = bob_Confirm2FromAlice->sequenceNumber;
		bob_Confirm2FromAlice_message = (bzrtpConfirmMessage_t *)bob_Confirm2FromAlice->messageData;
		memcpy(contextBob->channelContext[0]->peerH[0], bob_Confirm2FromAlice_message->H0, 32);
		 
		contextBob->isSecure = 1;
	}

	packetDump(alice_Confirm2,1);
	packetDump(bob_Confirm2FromAlice,0);

	bzrtp_freeZrtpPacket(bob_Confirm2FromAlice);
	bzrtp_freeZrtpPacket(alice_Confirm2);

	 
	bob_Conf2ACK =  bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[0], MSGTYPE_CONF2ACK, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[0], bob_Conf2ACK, contextBob->channelContext[0]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[0]->selfSequenceNumber++;
	}
	bzrtp_message("Bob building Conf2ACK return %x\n", retval);

	alice_Conf2ACKFromBob = bzrtp_packetCheck(bob_Conf2ACK->packetString, bob_Conf2ACK->messageLength+16, contextAlice->channelContext[0]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_Conf2ACK->packetString, bob_Conf2ACK->messageLength+16, alice_Conf2ACKFromBob);
	bzrtp_message ("Alice parsing conf2ACK returns %x\n", retval);
	if (retval==0) {
		 
		contextAlice->channelContext[0]->peerSequenceNumber = alice_Conf2ACKFromBob->sequenceNumber;
		 
		contextAlice->isSecure = 1;
	}

	bzrtp_freeZrtpPacket(bob_Conf2ACK);
	bzrtp_freeZrtpPacket(alice_Conf2ACKFromBob);

	dumpContext("Alice", contextAlice);
	dumpContext("Bob", contextBob);

	bzrtp_message("\n\n\n\n\n*************************************************************\n        SECOND CHANNEL\n**********************************************\n\n");
	 
	retval = bzrtp_addChannel(contextAlice, 0x45678901);
	bzrtp_message("Add channel to Alice's context returns %d\n", retval);
	retval = bzrtp_addChannel(contextBob, 0x54321098);
	bzrtp_message("Add channel to Bob's context returns %d\n", retval);

	 
	alice_Hello = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[1], MSGTYPE_HELLO, &retval);
	if (bzrtp_packetBuild(contextAlice, contextAlice->channelContext[1], alice_Hello, contextAlice->channelContext[1]->selfSequenceNumber) ==0) {
		contextAlice->channelContext[1]->selfSequenceNumber++;
		contextAlice->channelContext[1]->selfPackets[HELLO_MESSAGE_STORE_ID] = alice_Hello;
	}
	bob_Hello = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[1], MSGTYPE_HELLO, &retval);
	if (bzrtp_packetBuild(contextBob, contextBob->channelContext[1], bob_Hello, contextBob->channelContext[1]->selfSequenceNumber) ==0) {
		contextBob->channelContext[1]->selfSequenceNumber++;
		contextBob->channelContext[1]->selfPackets[HELLO_MESSAGE_STORE_ID] = bob_Hello;
	}

	 
	alice_HelloFromBob = bzrtp_packetCheck(bob_Hello->packetString, bob_Hello->messageLength+16, contextAlice->channelContext[1]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextAlice, contextAlice->channelContext[0], bob_Hello->packetString, bob_Hello->messageLength+16, alice_HelloFromBob);
	bzrtp_message ("Alice parsing returns %x\n", retval);
	if (retval==0) {
		bzrtpHelloMessage_t *alice_HelloFromBob_message;
		int i;
		uint8_t checkPeerSupportMultiChannel = 0;

		contextAlice->channelContext[1]->peerSequenceNumber = alice_HelloFromBob->sequenceNumber;
		 
		contextAlice->channelContext[1]->peerPackets[HELLO_MESSAGE_STORE_ID] = alice_HelloFromBob;

		 
		alice_HelloFromBob_message = (bzrtpHelloMessage_t *)alice_HelloFromBob->messageData;
		for (i=0; i<alice_HelloFromBob_message->kc; i++) {
			if (alice_HelloFromBob_message->supportedKeyAgreement[i] == ZRTP_KEYAGREEMENT_Mult) {
				checkPeerSupportMultiChannel = 1;
			}
		}

		 
		if (checkPeerSupportMultiChannel == 1) {
			bzrtp_message("Alice found that Bob supports multi channel\n");
			 
			contextAlice->channelContext[1]->hashAlgo = contextAlice->channelContext[0]->hashAlgo;
			contextAlice->channelContext[1]->hashLength = contextAlice->channelContext[0]->hashLength;
			contextAlice->channelContext[1]->cipherAlgo = contextAlice->channelContext[0]->cipherAlgo;
			contextAlice->channelContext[1]->cipherKeyLength = contextAlice->channelContext[0]->cipherKeyLength;
			contextAlice->channelContext[1]->authTagAlgo = contextAlice->channelContext[0]->authTagAlgo;
			contextAlice->channelContext[1]->sasAlgo = contextAlice->channelContext[0]->sasAlgo;
			contextAlice->channelContext[1]->keyAgreementAlgo = ZRTP_KEYAGREEMENT_Mult;
			contextAlice->channelContext[1]->keyAgreementLength = 0;  

			updateCryptoFunctionPointers(contextAlice->channelContext[1]);
		} else {
			bzrtp_message("ERROR : Alice found that Bob doesn't support multi channel\n");
		}

	}

	bob_HelloFromAlice = bzrtp_packetCheck(alice_Hello->packetString, alice_Hello->messageLength+16, contextBob->channelContext[1]->peerSequenceNumber, &retval);
	retval +=  bzrtp_packetParser(contextBob, contextBob->channelContext[1], alice_Hello->packetString, alice_Hello->messageLength+16, bob_HelloFromAlice);
	bzrtp_message ("Bob parsing returns %x\n", retval);
	if (retval==0) {
		bzrtpHelloMessage_t *bob_HelloFromAlice_message;
		int i;
		uint8_t checkPeerSupportMultiChannel = 0;

		contextBob->channelContext[1]->peerSequenceNumber = bob_HelloFromAlice->sequenceNumber;
		 
		contextBob->channelContext[1]->peerPackets[HELLO_MESSAGE_STORE_ID] = bob_HelloFromAlice;

		 
		bob_HelloFromAlice_message = (bzrtpHelloMessage_t *)bob_HelloFromAlice->messageData;
		for (i=0; i<bob_HelloFromAlice_message->kc; i++) {
			if (bob_HelloFromAlice_message->supportedKeyAgreement[i] == ZRTP_KEYAGREEMENT_Mult) {
				checkPeerSupportMultiChannel = 1;
			}
		}

		 
		if (checkPeerSupportMultiChannel == 1) {
			bzrtp_message("Bob found that Alice supports multi channel\n");
			 
			contextBob->channelContext[1]->hashAlgo = contextBob->channelContext[0]->hashAlgo;
			contextBob->channelContext[1]->hashLength = contextBob->channelContext[0]->hashLength;
			contextBob->channelContext[1]->cipherAlgo = contextBob->channelContext[0]->cipherAlgo;
			contextBob->channelContext[1]->cipherKeyLength = contextBob->channelContext[0]->cipherKeyLength;
			contextBob->channelContext[1]->authTagAlgo = contextBob->channelContext[0]->authTagAlgo;
			contextBob->channelContext[1]->sasAlgo = contextBob->channelContext[0]->sasAlgo;
			contextBob->channelContext[1]->keyAgreementAlgo = ZRTP_KEYAGREEMENT_Mult;
			contextBob->channelContext[1]->keyAgreementLength = 0;  

			updateCryptoFunctionPointers(contextBob->channelContext[1]);
		} else {
			bzrtp_message("ERROR : Bob found that Alice doesn't support multi channel\n");
		}
	}

	 
	alice_HelloFromBob_message = (bzrtpHelloMessage_t *)alice_HelloFromBob->messageData;
	memcpy(contextAlice->channelContext[1]->peerH[3], alice_HelloFromBob_message->H3, 32);
	bob_HelloFromAlice_message = (bzrtpHelloMessage_t *)bob_HelloFromAlice->messageData;
	memcpy(contextBob->channelContext[1]->peerH[3], bob_HelloFromAlice_message->H3, 32);


	 

	 
	bob_Commit = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[1], MSGTYPE_COMMIT, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[1], bob_Commit, contextBob->channelContext[1]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[1]->selfSequenceNumber++;
		contextBob->channelContext[1]->selfPackets[COMMIT_MESSAGE_STORE_ID] = bob_Commit;
	}
	bzrtp_message("Bob building Commit return %x\n", retval);


	 
	alice_CommitFromBob = bzrtp_packetCheck(bob_Commit->packetString, bob_Commit->messageLength+16, contextAlice->channelContext[1]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[1], bob_Commit->packetString, bob_Commit->messageLength+16, alice_CommitFromBob);
	bzrtp_message ("Alice parsing Commit returns %x\n", retval);
	if (retval==0) {
		bzrtpCommitMessage_t *alice_CommitFromBob_message;

		 
		contextAlice->channelContext[1]->peerSequenceNumber = alice_CommitFromBob->sequenceNumber;
		 
		alice_CommitFromBob_message = (bzrtpCommitMessage_t *)alice_CommitFromBob->messageData;
		memcpy(contextAlice->channelContext[1]->peerH[2], alice_CommitFromBob_message->H2, 32);
		contextAlice->channelContext[1]->peerPackets[COMMIT_MESSAGE_STORE_ID] = alice_CommitFromBob;
	}
	packetDump(alice_CommitFromBob, 0);

	 
	contextAlice->channelContext[1]->role = RESPONDER;

	 
	totalHashDataLength = alice_Hello->messageLength + bob_Commit->messageLength;
	dataToHash = (uint8_t *)malloc(totalHashDataLength*sizeof(uint8_t));
	hashDataIndex = 0;

	 
	memcpy(dataToHash, contextAlice->channelContext[1]->selfPackets[HELLO_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[1]->selfPackets[HELLO_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextAlice->channelContext[1]->selfPackets[HELLO_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextAlice->channelContext[1]->peerPackets[COMMIT_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextAlice->channelContext[1]->peerPackets[COMMIT_MESSAGE_STORE_ID]->messageLength);
	contextAlice->channelContext[1]->hashFunction(dataToHash, totalHashDataLength, 32, alice_totalHash);

	 
	hashDataIndex = 0;
	memcpy(dataToHash, contextBob->channelContext[1]->peerPackets[HELLO_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[1]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageLength);
	hashDataIndex += contextBob->channelContext[1]->peerPackets[HELLO_MESSAGE_STORE_ID]->messageLength;
	memcpy(dataToHash+hashDataIndex, contextBob->channelContext[1]->selfPackets[COMMIT_MESSAGE_STORE_ID]->packetString+ZRTP_PACKET_HEADER_LENGTH, contextBob->channelContext[1]->selfPackets[COMMIT_MESSAGE_STORE_ID]->messageLength);

	contextBob->channelContext[1]->hashFunction(dataToHash, totalHashDataLength, 32, bob_totalHash);
	if (memcmp(bob_totalHash, alice_totalHash, 32) == 0) {
		bzrtp_message("Got the same total hash\n");
		CU_PASS("Total Hash match");
	} else {
		bzrtp_message("AARGG!! total hash mismatch");
		CU_FAIL("Total Hash mismatch");
	}

	free(dataToHash);


	 
	contextAlice->channelContext[1]->KDFContextLength = 24 + contextAlice->channelContext[1]->hashLength;
	contextAlice->channelContext[1]->KDFContext = (uint8_t *)malloc(contextAlice->channelContext[1]->KDFContextLength*sizeof(uint8_t));
	memcpy(contextAlice->channelContext[1]->KDFContext, contextAlice->peerZID, 12);
	memcpy(contextAlice->channelContext[1]->KDFContext+12, contextAlice->selfZID, 12);
	memcpy(contextAlice->channelContext[1]->KDFContext+24, alice_totalHash, contextAlice->channelContext[1]->hashLength);

	contextBob->channelContext[1]->KDFContextLength = 24 + contextBob->channelContext[1]->hashLength;
	contextBob->channelContext[1]->KDFContext = (uint8_t *)malloc(contextBob->channelContext[1]->KDFContextLength*sizeof(uint8_t));
	memcpy(contextBob->channelContext[1]->KDFContext, contextBob->selfZID, 12);
	memcpy(contextBob->channelContext[1]->KDFContext+12, contextBob->peerZID, 12);
	memcpy(contextBob->channelContext[1]->KDFContext+24, bob_totalHash, contextBob->channelContext[1]->hashLength);

	if (memcmp(contextBob->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContext, 56) == 0) {
		bzrtp_message("Got the same total KDF Context\n");
		CU_PASS("KDFContext match");
	} else {
		bzrtp_message("AARGG!! KDF Context mismatch");
		CU_FAIL("KDF Context mismatch");
	}

	 
	contextBob->channelContext[1]->s0 = (uint8_t *)malloc(contextBob->channelContext[1]->hashLength*sizeof(uint8_t));
	contextAlice->channelContext[1]->s0 = (uint8_t *)malloc(contextAlice->channelContext[1]->hashLength*sizeof(uint8_t));
	retval = bzrtp_keyDerivationFunction(contextBob->ZRTPSess, contextBob->ZRTPSessLength,
		(uint8_t *)"ZRTP MSK", 8,
		contextBob->channelContext[1]->KDFContext, contextBob->channelContext[1]->KDFContextLength,  
		contextBob->channelContext[1]->hashLength,
		(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[1]->hmacFunction,
		contextBob->channelContext[1]->s0);

	retval = bzrtp_keyDerivationFunction(contextAlice->ZRTPSess, contextAlice->ZRTPSessLength,
		(uint8_t *)"ZRTP MSK", 8,
		contextAlice->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContextLength,  
		contextAlice->channelContext[1]->hashLength,
		(void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[1]->hmacFunction,
		contextAlice->channelContext[1]->s0);

	if (memcmp(contextBob->channelContext[1]->s0, contextAlice->channelContext[1]->s0, contextAlice->channelContext[1]->hashLength) == 0) {
		bzrtp_message("Got the same s0\n");
		CU_PASS("s0 match");
	} else {
		bzrtp_message("AARGG!! s0 mismatch");
		CU_FAIL("s0 mismatch");
	}


	 
	 
	contextAlice->channelContext[1]->mackeyi = (uint8_t *)malloc(contextAlice->channelContext[1]->hashLength*(sizeof(uint8_t)));
	contextAlice->channelContext[1]->mackeyr = (uint8_t *)malloc(contextAlice->channelContext[1]->hashLength*(sizeof(uint8_t)));
	contextAlice->channelContext[1]->zrtpkeyi = (uint8_t *)malloc(contextAlice->channelContext[1]->cipherKeyLength*(sizeof(uint8_t)));
	contextAlice->channelContext[1]->zrtpkeyr = (uint8_t *)malloc(contextAlice->channelContext[1]->cipherKeyLength*(sizeof(uint8_t)));
	contextBob->channelContext[1]->mackeyi = (uint8_t *)malloc(contextBob->channelContext[1]->hashLength*(sizeof(uint8_t)));
	contextBob->channelContext[1]->mackeyr = (uint8_t *)malloc(contextBob->channelContext[1]->hashLength*(sizeof(uint8_t)));
	contextBob->channelContext[1]->zrtpkeyi = (uint8_t *)malloc(contextBob->channelContext[1]->cipherKeyLength*(sizeof(uint8_t)));
	contextBob->channelContext[1]->zrtpkeyr = (uint8_t *)malloc(contextBob->channelContext[1]->cipherKeyLength*(sizeof(uint8_t)));

	 
	retval = bzrtp_keyDerivationFunction(contextAlice->channelContext[1]->s0, contextAlice->channelContext[1]->hashLength, (uint8_t *)"Initiator HMAC key", 18, contextAlice->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContextLength, contextAlice->channelContext[1]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[1]->hmacFunction, contextAlice->channelContext[1]->mackeyi);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[1]->s0, contextAlice->channelContext[1]->hashLength, (uint8_t *)"Responder HMAC key", 18, contextAlice->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContextLength, contextAlice->channelContext[1]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[1]->hmacFunction, contextAlice->channelContext[1]->mackeyr);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[1]->s0, contextAlice->channelContext[1]->hashLength, (uint8_t *)"Initiator ZRTP key", 18, contextAlice->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContextLength, contextAlice->channelContext[1]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[1]->hmacFunction, contextAlice->channelContext[1]->zrtpkeyi);
	retval += bzrtp_keyDerivationFunction(contextAlice->channelContext[1]->s0, contextAlice->channelContext[1]->hashLength, (uint8_t *)"Responder ZRTP key", 18, contextAlice->channelContext[1]->KDFContext, contextAlice->channelContext[1]->KDFContextLength, contextAlice->channelContext[1]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextAlice->channelContext[1]->hmacFunction, contextAlice->channelContext[1]->zrtpkeyr);

	 
	retval = bzrtp_keyDerivationFunction(contextBob->channelContext[1]->s0, contextBob->channelContext[1]->hashLength, (uint8_t *)"Initiator HMAC key", 18, contextBob->channelContext[1]->KDFContext, contextBob->channelContext[1]->KDFContextLength, contextBob->channelContext[1]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[1]->hmacFunction, contextBob->channelContext[1]->mackeyi);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[1]->s0, contextBob->channelContext[1]->hashLength, (uint8_t *)"Responder HMAC key", 18, contextBob->channelContext[1]->KDFContext, contextBob->channelContext[1]->KDFContextLength, contextBob->channelContext[1]->hashLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[1]->hmacFunction, contextBob->channelContext[1]->mackeyr);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[1]->s0, contextBob->channelContext[1]->hashLength, (uint8_t *)"Initiator ZRTP key", 18, contextBob->channelContext[1]->KDFContext, contextBob->channelContext[1]->KDFContextLength, contextBob->channelContext[1]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[1]->hmacFunction, contextBob->channelContext[1]->zrtpkeyi);
	retval += bzrtp_keyDerivationFunction(contextBob->channelContext[1]->s0, contextBob->channelContext[1]->hashLength, (uint8_t *)"Responder ZRTP key", 18, contextBob->channelContext[1]->KDFContext, contextBob->channelContext[1]->KDFContextLength, contextBob->channelContext[1]->cipherKeyLength, (void (*)(uint8_t *, uint8_t,  uint8_t *, uint32_t,  uint8_t,  uint8_t *))contextBob->channelContext[1]->hmacFunction, contextBob->channelContext[1]->zrtpkeyr);

	 
	if ((memcmp(contextAlice->channelContext[1]->mackeyi, contextBob->channelContext[1]->mackeyi, contextAlice->channelContext[1]->hashLength)==0) && (memcmp(contextAlice->channelContext[1]->mackeyr, contextBob->channelContext[1]->mackeyr, contextAlice->channelContext[1]->hashLength)==0) && (memcmp(contextAlice->channelContext[1]->zrtpkeyi, contextBob->channelContext[1]->zrtpkeyi, contextAlice->channelContext[1]->cipherKeyLength)==0) && (memcmp(contextAlice->channelContext[1]->zrtpkeyr, contextBob->channelContext[1]->zrtpkeyr, contextAlice->channelContext[1]->cipherKeyLength)==0)) {
		bzrtp_message("Got the same keys\n");
		CU_PASS("keys match");
	} else {
		bzrtp_message("ERROR keys differ\n");
		CU_PASS("Keys mismatch");
	}

	 
	alice_Confirm1 = bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[1], MSGTYPE_CONFIRM1, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[1], alice_Confirm1, contextAlice->channelContext[1]->selfSequenceNumber);
	if (retval == 0) {
		contextAlice->channelContext[1]->selfSequenceNumber++;
	}
	bzrtp_message("Alice building Confirm1 return %x\n", retval);

	bob_Confirm1FromAlice = bzrtp_packetCheck(alice_Confirm1->packetString, alice_Confirm1->messageLength+16, contextBob->channelContext[1]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextBob, contextBob->channelContext[1], alice_Confirm1->packetString, alice_Confirm1->messageLength+16, bob_Confirm1FromAlice);
	bzrtp_message ("Bob parsing confirm1 returns %x\n", retval);
	if (retval==0) {
		 
		contextBob->channelContext[1]->peerSequenceNumber = bob_Confirm1FromAlice->sequenceNumber;
		bob_Confirm1FromAlice_message = (bzrtpConfirmMessage_t *)bob_Confirm1FromAlice->messageData;
		memcpy(contextBob->channelContext[1]->peerH[0], bob_Confirm1FromAlice_message->H0, 32);
	}

	packetDump(bob_Confirm1FromAlice,0);
	bzrtp_freeZrtpPacket(bob_Confirm1FromAlice);
	bzrtp_freeZrtpPacket(alice_Confirm1);

	 
	bob_Confirm2 = bzrtp_createZrtpPacket(contextBob, contextBob->channelContext[1], MSGTYPE_CONFIRM2, &retval);
	retval += bzrtp_packetBuild(contextBob, contextBob->channelContext[1], bob_Confirm2, contextBob->channelContext[1]->selfSequenceNumber);
	if (retval == 0) {
		contextBob->channelContext[1]->selfSequenceNumber++;
	}
	bzrtp_message("Bob building Confirm2 return %x\n", retval);
	alice_Confirm2FromBob = bzrtp_packetCheck(bob_Confirm2->packetString, bob_Confirm2->messageLength+16, contextAlice->channelContext[1]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextAlice, contextAlice->channelContext[1], bob_Confirm2->packetString, bob_Confirm2->messageLength+16, alice_Confirm2FromBob);
	bzrtp_message ("Alice parsing confirm2 returns %x\n", retval);
	if (retval==0) {
		 
		contextAlice->channelContext[1]->peerSequenceNumber = alice_Confirm2FromBob->sequenceNumber;
		alice_Confirm2FromBob_message = (bzrtpConfirmMessage_t *)alice_Confirm2FromBob->messageData;
		memcpy(contextAlice->channelContext[1]->peerH[0], alice_Confirm2FromBob_message->H0, 32);
	}

	packetDump(alice_Confirm2FromBob,0);
	bzrtp_freeZrtpPacket(alice_Confirm2FromBob);
	bzrtp_freeZrtpPacket(bob_Confirm2);

	 
	alice_Conf2ACK =  bzrtp_createZrtpPacket(contextAlice, contextAlice->channelContext[1], MSGTYPE_CONF2ACK, &retval);
	retval += bzrtp_packetBuild(contextAlice, contextAlice->channelContext[1], alice_Conf2ACK, contextAlice->channelContext[1]->selfSequenceNumber);
	if (retval == 0) {
		contextAlice->channelContext[1]->selfSequenceNumber++;
	}
	bzrtp_message("Alice building Conf2ACK return %x\n", retval);

	bob_Conf2ACKFromAlice = bzrtp_packetCheck(alice_Conf2ACK->packetString, alice_Conf2ACK->messageLength+16, contextBob->channelContext[1]->peerSequenceNumber, &retval);
	retval += bzrtp_packetParser(contextBob, contextBob->channelContext[1], alice_Conf2ACK->packetString, alice_Conf2ACK->messageLength+16, bob_Conf2ACKFromAlice);
	bzrtp_message ("Bob parsing conf2ACK returns %x\n", retval);
	if (retval==0) {
		 
		contextBob->channelContext[1]->peerSequenceNumber = bob_Conf2ACKFromAlice->sequenceNumber;
	}


	bzrtp_freeZrtpPacket(alice_Conf2ACK);
	bzrtp_freeZrtpPacket(bob_Conf2ACKFromAlice);





 
	bzrtp_message("Destroy the contexts\n");
	 
	bzrtp_destroyBzrtpContext(contextAlice, 0x45678901);
	bzrtp_destroyBzrtpContext(contextBob, 0x54321098);
	bzrtp_message("Destroy the contexts last channel\n");
	bzrtp_destroyBzrtpContext(contextBob, 0x87654321);
	bzrtp_destroyBzrtpContext(contextAlice, 0x12345678);

}

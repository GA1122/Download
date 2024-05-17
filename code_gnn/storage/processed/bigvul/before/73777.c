void bzrtp_freeZrtpPacket(bzrtpPacket_t *zrtpPacket) {
	if (zrtpPacket != NULL) {
		 
		if (zrtpPacket->messageData != NULL) {
			switch(zrtpPacket->messageType) {
				case MSGTYPE_DHPART1 :
				case MSGTYPE_DHPART2 :
					{
						bzrtpDHPartMessage_t *typedMessageData = (bzrtpDHPartMessage_t *)(zrtpPacket->messageData);
						if (typedMessageData != NULL) {
							free(typedMessageData->pv);
						}
					}
					break;
				case MSGTYPE_CONFIRM1:
				case MSGTYPE_CONFIRM2:
					{
						bzrtpConfirmMessage_t *typedMessageData = (bzrtpConfirmMessage_t *)(zrtpPacket->messageData);
						if (typedMessageData != NULL) {
							free(typedMessageData->signatureBlock);
						}
					}
					break;
			}
		}
		free(zrtpPacket->messageData);
		free(zrtpPacket->packetString);
		free(zrtpPacket);
	}
}
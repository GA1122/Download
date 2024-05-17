static int CheckOtherHTTPHeaders(
	 
	http_message_t *Req,
	 
	struct SendInstruction *RespInstr,
	 
	off_t FileSize)
{
	http_header_t *header;
	ListNode *node;
	 
	int index, RetCode = HTTP_OK;
	char *TmpBuf;
	size_t TmpBufSize = LINE_SIZE;

	TmpBuf = (char *)malloc(TmpBufSize);
	if (!TmpBuf)
		return HTTP_INTERNAL_SERVER_ERROR;
	node = ListHead(&Req->headers);
	while (node != NULL) {
		header = (http_header_t *) node->item;
		 
		index = map_str_to_int((const char *)header->name.buf,
				header->name.length, Http_Header_Names,
				NUM_HTTP_HEADER_NAMES, FALSE);
		if (header->value.length >= TmpBufSize) {
			free(TmpBuf);
			TmpBufSize = header->value.length + 1;
			TmpBuf = (char *)malloc(TmpBufSize);
			if (!TmpBuf)
				return HTTP_INTERNAL_SERVER_ERROR;
		}
		memcpy(TmpBuf, header->value.buf, header->value.length);
		TmpBuf[header->value.length] = '\0';
		if (index >= 0) {
			switch (Http_Header_Names[index].id) {
			case HDR_TE: {
				 
				RespInstr->IsChunkActive = 1;

				if (strlen(TmpBuf) > strlen("gzip")) {
					 
					if (StrStr(TmpBuf, "trailers") != NULL) {
						RespInstr->IsTrailers = 1;
					}
				}
				break;
			}
			case HDR_CONTENT_LENGTH:
				RespInstr->RecvWriteSize = atoi(TmpBuf);
				break;
			case HDR_RANGE:
				RetCode = CreateHTTPRangeResponseHeader(TmpBuf,
					FileSize, RespInstr);
				if (RetCode != HTTP_OK) {
					free(TmpBuf);
					return RetCode;
				}
				break;
			case HDR_ACCEPT_LANGUAGE:
				if (header->value.length + 1 > sizeof(RespInstr->AcceptLanguageHeader)) {
					size_t length = sizeof(RespInstr->AcceptLanguageHeader) - 1;
					memcpy(RespInstr->AcceptLanguageHeader, TmpBuf, length);
					RespInstr->AcceptLanguageHeader[length] = '\0';
				} else {
					memcpy(RespInstr->AcceptLanguageHeader, TmpBuf,
						header->value.length + 1);
				}
				break;
			default:
				 
				 
				 

				 
				 

				 
				 
				break;
			}
		}
		node = ListNext(&Req->headers, node);
	}
	free(TmpBuf);

	return RetCode;
}

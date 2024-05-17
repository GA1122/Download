SECURITY_STATUS credssp_decrypt_ts_credentials(rdpCredssp* credssp)
{
	int length;
	BYTE* buffer;
	ULONG pfQOP;
	SecBuffer Buffers[2];
	SecBufferDesc Message;
	SECURITY_STATUS status;

	Buffers[0].BufferType = SECBUFFER_TOKEN;  
	Buffers[1].BufferType = SECBUFFER_DATA;  

	if (credssp->authInfo.cbBuffer < 1)
	{
		fprintf(stderr, "credssp_decrypt_ts_credentials missing authInfo buffer\n");
		return SEC_E_INVALID_TOKEN;
	}

	length = credssp->authInfo.cbBuffer;
	buffer = (BYTE*) malloc(length);
	CopyMemory(buffer, credssp->authInfo.pvBuffer, length);

	Buffers[0].cbBuffer = credssp->ContextSizes.cbMaxSignature;
	Buffers[0].pvBuffer = buffer;

	Buffers[1].cbBuffer = length - credssp->ContextSizes.cbMaxSignature;
	Buffers[1].pvBuffer = &buffer[credssp->ContextSizes.cbMaxSignature];

	Message.cBuffers = 2;
	Message.ulVersion = SECBUFFER_VERSION;
	Message.pBuffers = (PSecBuffer) &Buffers;

	status = credssp->table->DecryptMessage(&credssp->context, &Message, credssp->recv_seq_num++, &pfQOP);

	if (status != SEC_E_OK)
		return status;

	credssp_read_ts_credentials(credssp, &Buffers[1]);

	free(buffer);

	return SEC_E_OK;
}
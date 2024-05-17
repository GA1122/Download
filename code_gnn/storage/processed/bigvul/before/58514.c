SECURITY_STATUS credssp_decrypt_public_key_echo(rdpCredssp* credssp)
{
	int length;
	BYTE* buffer;
	ULONG pfQOP;
	BYTE* public_key1;
	BYTE* public_key2;
	int public_key_length;
	SecBuffer Buffers[2];
	SecBufferDesc Message;
	SECURITY_STATUS status;

	if (credssp->PublicKey.cbBuffer + credssp->ContextSizes.cbMaxSignature != credssp->pubKeyAuth.cbBuffer)
	{
		fprintf(stderr, "unexpected pubKeyAuth buffer size:%d\n", (int) credssp->pubKeyAuth.cbBuffer);
		return SEC_E_INVALID_TOKEN;
	}

	length = credssp->pubKeyAuth.cbBuffer;
	buffer = (BYTE*) malloc(length);
	CopyMemory(buffer, credssp->pubKeyAuth.pvBuffer, length);

	public_key_length = credssp->PublicKey.cbBuffer;

	Buffers[0].BufferType = SECBUFFER_TOKEN;  
	Buffers[1].BufferType = SECBUFFER_DATA;  

	Buffers[0].cbBuffer = credssp->ContextSizes.cbMaxSignature;
	Buffers[0].pvBuffer = buffer;

	Buffers[1].cbBuffer = length - credssp->ContextSizes.cbMaxSignature;
	Buffers[1].pvBuffer = buffer + credssp->ContextSizes.cbMaxSignature;

	Message.cBuffers = 2;
	Message.ulVersion = SECBUFFER_VERSION;
	Message.pBuffers = (PSecBuffer) &Buffers;

	status = credssp->table->DecryptMessage(&credssp->context, &Message, credssp->recv_seq_num++, &pfQOP);

	if (status != SEC_E_OK)
	{
		fprintf(stderr, "DecryptMessage failure: 0x%08X\n", status);
		return status;
	}

	public_key1 = (BYTE*) credssp->PublicKey.pvBuffer;
	public_key2 = (BYTE*) Buffers[1].pvBuffer;

	if (!credssp->server)
	{
		 
		ap_integer_decrement_le(public_key2, public_key_length);
	}

	if (memcmp(public_key1, public_key2, public_key_length) != 0)
	{
		fprintf(stderr, "Could not verify server's public key echo\n");

		fprintf(stderr, "Expected (length = %d):\n", public_key_length);
		winpr_HexDump(public_key1, public_key_length);

		fprintf(stderr, "Actual (length = %d):\n", public_key_length);
		winpr_HexDump(public_key2, public_key_length);

		return SEC_E_MESSAGE_ALTERED;  
	}

	free(buffer);

	return SEC_E_OK;
}
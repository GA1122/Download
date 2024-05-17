SECURITY_STATUS credssp_encrypt_public_key_echo(rdpCredssp* credssp)
{
	SecBuffer Buffers[2];
	SecBufferDesc Message;
	SECURITY_STATUS status;
	int public_key_length;

	public_key_length = credssp->PublicKey.cbBuffer;

	Buffers[0].BufferType = SECBUFFER_TOKEN;  
	Buffers[1].BufferType = SECBUFFER_DATA;  

	sspi_SecBufferAlloc(&credssp->pubKeyAuth, credssp->ContextSizes.cbMaxSignature + public_key_length);

	Buffers[0].cbBuffer = credssp->ContextSizes.cbMaxSignature;
	Buffers[0].pvBuffer = credssp->pubKeyAuth.pvBuffer;

	Buffers[1].cbBuffer = public_key_length;
	Buffers[1].pvBuffer = ((BYTE*) credssp->pubKeyAuth.pvBuffer) + credssp->ContextSizes.cbMaxSignature;
	CopyMemory(Buffers[1].pvBuffer, credssp->PublicKey.pvBuffer, Buffers[1].cbBuffer);

	if (credssp->server)
	{
		 
		ap_integer_increment_le((BYTE*) Buffers[1].pvBuffer, Buffers[1].cbBuffer);
	}

	Message.cBuffers = 2;
	Message.ulVersion = SECBUFFER_VERSION;
	Message.pBuffers = (PSecBuffer) &Buffers;

	status = credssp->table->EncryptMessage(&credssp->context, 0, &Message, credssp->send_seq_num++);

	if (status != SEC_E_OK)
	{
		fprintf(stderr, "EncryptMessage status: 0x%08X\n", status);
		return status;
	}

	return status;
}
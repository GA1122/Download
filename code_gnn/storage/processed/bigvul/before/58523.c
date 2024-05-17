int credssp_recv(rdpCredssp* credssp)
{
	wStream* s;
	int length;
	int status;
	UINT32 version;

	s = Stream_New(NULL, 4096);

	status = transport_read(credssp->transport, s);
	Stream_Length(s) = status;

	if (status < 0)
	{
		fprintf(stderr, "credssp_recv() error: %d\n", status);
		Stream_Free(s, TRUE);
		return -1;
	}

	 
	if(!ber_read_sequence_tag(s, &length) ||
		!ber_read_contextual_tag(s, 0, &length, TRUE) ||
		!ber_read_integer(s, &version))
		return -1;

	 
	if (ber_read_contextual_tag(s, 1, &length, TRUE) != FALSE)
	{
		if (!ber_read_sequence_tag(s, &length) ||  
			!ber_read_sequence_tag(s, &length) ||  
			!ber_read_contextual_tag(s, 0, &length, TRUE) ||  
			!ber_read_octet_string_tag(s, &length) ||  
			Stream_GetRemainingLength(s) < length)
			return -1;
		sspi_SecBufferAlloc(&credssp->negoToken, length);
		Stream_Read(s, credssp->negoToken.pvBuffer, length);
		credssp->negoToken.cbBuffer = length;
	}

	 
	if (ber_read_contextual_tag(s, 2, &length, TRUE) != FALSE)
	{
		if(!ber_read_octet_string_tag(s, &length) ||  
			Stream_GetRemainingLength(s) < length)
			return -1;
		sspi_SecBufferAlloc(&credssp->authInfo, length);
		Stream_Read(s, credssp->authInfo.pvBuffer, length);
		credssp->authInfo.cbBuffer = length;
	}

	 
	if (ber_read_contextual_tag(s, 3, &length, TRUE) != FALSE)
	{
		if(!ber_read_octet_string_tag(s, &length) ||  
			Stream_GetRemainingLength(s) < length)
			return -1;
		sspi_SecBufferAlloc(&credssp->pubKeyAuth, length);
		Stream_Read(s, credssp->pubKeyAuth.pvBuffer, length);
		credssp->pubKeyAuth.cbBuffer = length;
	}

	Stream_Free(s, TRUE);

	return 0;
}
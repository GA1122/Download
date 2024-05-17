Condor_Auth_SSL::encrypt_or_decrypt(bool want_encrypt, 
									   unsigned char* input, 
									   int input_len, 
									   unsigned char* &output, 
									   int &output_len)
{
	bool result;
	
	if ( output ) free(output);
	output = NULL;
	output_len = 0;
	
	if (!input || input_len < 1) {
		return false;
	}
	
	if (!m_crypto) {
		return false;
	}

	m_crypto->resetState();
	if (want_encrypt) {
		result = m_crypto->encrypt(input,input_len,output,output_len);
	} else {
		result = m_crypto->decrypt(input,input_len,output,output_len);
	}
	
	if (!result) {
		output_len = 0;
	}

	if ( output_len == 0 ) {
		if ( output ) free(output);
		output = NULL;
		return false;
	} 
	
	return true;
}

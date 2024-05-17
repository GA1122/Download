Condor_Auth_SSL::encrypt(unsigned char* input, 
					int input_len, unsigned char* & output, int& output_len)
{
	return encrypt_or_decrypt(true,input,input_len,output,output_len);
}

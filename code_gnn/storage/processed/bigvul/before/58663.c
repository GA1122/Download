void security_licensing_encryption_key(const BYTE* session_key_blob, const BYTE* client_random,
		const BYTE* server_random, BYTE* output)
{
	 
	security_md5_16_32_32(&session_key_blob[16], client_random, server_random, output);
}

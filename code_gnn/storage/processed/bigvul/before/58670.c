static void security_premaster_hash(const char* input, int length, const BYTE* premaster_secret, const BYTE* client_random, const BYTE* server_random, BYTE* output)
{
	 
	security_salted_hash(premaster_secret, (BYTE*)input, length, client_random, server_random, output);
}

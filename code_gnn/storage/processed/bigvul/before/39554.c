void license_generate_randoms(rdpLicense* license)
{
	ZeroMemory(license->ClientRandom, CLIENT_RANDOM_LENGTH);  
	ZeroMemory(license->PremasterSecret, PREMASTER_SECRET_LENGTH);  

#ifndef LICENSE_NULL_CLIENT_RANDOM
	crypto_nonce(license->ClientRandom, CLIENT_RANDOM_LENGTH);  
#endif

#ifndef LICENSE_NULL_PREMASTER_SECRET
	crypto_nonce(license->PremasterSecret, PREMASTER_SECRET_LENGTH);  
#endif
}

void license_write_platform_challenge_response_packet(rdpLicense* license, wStream* s, BYTE* macData)
{
	license_write_binary_blob(s, license->EncryptedPlatformChallenge);  
	license_write_binary_blob(s, license->EncryptedHardwareId);  

	Stream_EnsureRemainingCapacity(s, 16);
	Stream_Write(s, macData, 16);  
}

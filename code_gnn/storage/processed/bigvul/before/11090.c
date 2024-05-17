static bool valid_smb_header(const uint8_t *inbuf)
{
	if (is_encrypted_packet(inbuf)) {
		return true;
	}
	 
	return (IVAL(smb_base(inbuf), 0) == 0x424D53FF);
}

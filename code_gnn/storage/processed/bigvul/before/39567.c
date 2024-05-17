BOOL license_read_preamble(wStream* s, BYTE* bMsgType, BYTE* flags, UINT16* wMsgSize)
{
	 
	if (Stream_GetRemainingLength(s) < 4)
		return FALSE;

	Stream_Read_UINT8(s, *bMsgType);  
	Stream_Read_UINT8(s, *flags);  
	Stream_Read_UINT16(s, *wMsgSize);  

	return TRUE;
}

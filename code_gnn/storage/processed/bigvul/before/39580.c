void license_write_preamble(wStream* s, BYTE bMsgType, BYTE flags, UINT16 wMsgSize)
{
	 
	Stream_Write_UINT8(s, bMsgType);  
	Stream_Write_UINT8(s, flags);  
	Stream_Write_UINT16(s, wMsgSize);  
}

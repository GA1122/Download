static BOOL zgfx_compress_segment(ZGFX_CONTEXT* zgfx, wStream* s, const BYTE* pSrcData,
                                  UINT32 SrcSize, UINT32* pFlags)
{
	 
	if (!Stream_EnsureRemainingCapacity(s, SrcSize + 1))
	{
		WLog_ERR(TAG, "Stream_EnsureRemainingCapacity failed!");
		return FALSE;
	}

	(*pFlags) |= ZGFX_PACKET_COMPR_TYPE_RDP8;  
	Stream_Write_UINT8(s, (*pFlags));  
	Stream_Write(s, pSrcData, SrcSize);
	return TRUE;
}

 static BOOL nsc_stream_initialize(NSC_CONTEXT* context, wStream* s)
{
	int i;

	if (Stream_GetRemainingLength(s) < 20)
		return FALSE;

	for (i = 0; i < 4; i++)
		Stream_Read_UINT32(s, context->PlaneByteCount[i]);

	Stream_Read_UINT8(s, context->ColorLossLevel);  
	Stream_Read_UINT8(s,
	                  context->ChromaSubsamplingLevel);  
	Stream_Seek(s, 2);  
	context->Planes = Stream_Pointer(s);
	return TRUE;
}

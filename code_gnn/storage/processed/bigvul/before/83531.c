BOOL nsc_write_message(NSC_CONTEXT* context, wStream* s, NSC_MESSAGE* message)
{
	UINT32 totalPlaneByteCount;
	totalPlaneByteCount = message->LumaPlaneByteCount +
	                      message->OrangeChromaPlaneByteCount +
	                      message->GreenChromaPlaneByteCount + message->AlphaPlaneByteCount;

	if (!Stream_EnsureRemainingCapacity(s, 20 + totalPlaneByteCount))
		return -1;

	Stream_Write_UINT32(s,
	                    message->LumaPlaneByteCount);  
	Stream_Write_UINT32(s,
	                    message->OrangeChromaPlaneByteCount);  
	Stream_Write_UINT32(s,
	                    message->GreenChromaPlaneByteCount);  
	Stream_Write_UINT32(s,
	                    message->AlphaPlaneByteCount);  
	Stream_Write_UINT8(s, message->ColorLossLevel);  
	Stream_Write_UINT8(s,
	                   message->ChromaSubsamplingLevel);  
	Stream_Write_UINT16(s, 0);  

	if (message->LumaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[0],
		             message->LumaPlaneByteCount);  

	if (message->OrangeChromaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[1],
		             message->OrangeChromaPlaneByteCount);  

	if (message->GreenChromaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[2],
		             message->GreenChromaPlaneByteCount);  

	if (message->AlphaPlaneByteCount)
		Stream_Write(s, message->PlaneBuffers[3],
		             message->AlphaPlaneByteCount);  

	return TRUE;
}

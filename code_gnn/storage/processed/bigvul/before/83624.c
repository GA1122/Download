int zgfx_compress_to_stream(ZGFX_CONTEXT* zgfx, wStream* sDst, const BYTE* pUncompressed,
                            UINT32 uncompressedSize, UINT32* pFlags)
{
	int fragment;
	UINT16 maxLength;
	UINT32 totalLength;
	size_t posSegmentCount = 0;
	const BYTE* pSrcData;
	int status = 0;
	maxLength = ZGFX_SEGMENTED_MAXSIZE;
	totalLength = uncompressedSize;
	pSrcData = pUncompressed;

	for (fragment = 0; (totalLength > 0) || (fragment == 0); fragment++)
	{
		UINT32 SrcSize;
		size_t posDstSize;
		size_t posDataStart;
		UINT32 DstSize;
		SrcSize = (totalLength > maxLength) ? maxLength : totalLength;
		posDstSize = 0;
		totalLength -= SrcSize;

		 
		if (!Stream_EnsureRemainingCapacity(sDst, 12))
		{
			WLog_ERR(TAG, "Stream_EnsureRemainingCapacity failed!");
			return -1;
		}

		if (fragment == 0)
		{
			 
			 
			Stream_Write_UINT8(sDst, (totalLength == 0) ?
			                   ZGFX_SEGMENTED_SINGLE : ZGFX_SEGMENTED_MULTIPART);

			if (totalLength > 0)
			{
				posSegmentCount = Stream_GetPosition(sDst);  
				Stream_Seek(sDst, 2);
				Stream_Write_UINT32(sDst, uncompressedSize);  
			}
		}

		if (fragment > 0 || totalLength > 0)
		{
			 
			posDstSize = Stream_GetPosition(sDst);  
			Stream_Seek(sDst, 4);
		}

		posDataStart = Stream_GetPosition(sDst);

		if (!zgfx_compress_segment(zgfx, sDst, pSrcData, SrcSize, pFlags))
			return -1;

		if (posDstSize)
		{
			 
			DstSize = Stream_GetPosition(sDst) - posDataStart;
			Stream_SetPosition(sDst, posDstSize);
			Stream_Write_UINT32(sDst, DstSize);
			Stream_SetPosition(sDst, posDataStart + DstSize);
		}

		pSrcData += SrcSize;
	}

	Stream_SealLength(sDst);

	 
	if (posSegmentCount)
	{
		Stream_SetPosition(sDst, posSegmentCount);
		Stream_Write_UINT16(sDst, fragment);
		Stream_SetPosition(sDst, Stream_Length(sDst));
	}

	return status;
}

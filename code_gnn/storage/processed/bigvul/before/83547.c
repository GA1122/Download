static BOOL _update_read_pointer_color(wStream* s, POINTER_COLOR_UPDATE* pointer_color, BYTE xorBpp)
{
	BYTE* newMask;
	UINT32 scanlineSize;

	if (!pointer_color)
		goto fail;

	if (Stream_GetRemainingLength(s) < 14)
		goto fail;

	Stream_Read_UINT16(s, pointer_color->cacheIndex);  
	Stream_Read_UINT16(s, pointer_color->xPos);  
	Stream_Read_UINT16(s, pointer_color->yPos);  
	 
	Stream_Read_UINT16(s, pointer_color->width);  
	Stream_Read_UINT16(s, pointer_color->height);  

	if ((pointer_color->width > 96) || (pointer_color->height > 96))
		goto fail;

	Stream_Read_UINT16(s,
	                   pointer_color->lengthAndMask);  
	Stream_Read_UINT16(s,
	                   pointer_color->lengthXorMask);  

	 
	if (pointer_color->xPos >= pointer_color->width)
		pointer_color->xPos = 0;

	if (pointer_color->yPos >= pointer_color->height)
		pointer_color->yPos = 0;

	if (pointer_color->lengthXorMask > 0)
	{
		 
		if (Stream_GetRemainingLength(s) < pointer_color->lengthXorMask)
			goto fail;

		scanlineSize = (7 + xorBpp * pointer_color->width) / 8;
		scanlineSize = ((scanlineSize + 1) / 2) * 2;

		if (scanlineSize * pointer_color->height != pointer_color->lengthXorMask)
		{
			WLog_ERR(TAG,
			         "invalid lengthXorMask: width=%"PRIu32" height=%"PRIu32", %"PRIu32" instead of %"PRIu32"",
			         pointer_color->width, pointer_color->height,
			         pointer_color->lengthXorMask, scanlineSize * pointer_color->height);
			goto fail;
		}

		newMask = realloc(pointer_color->xorMaskData, pointer_color->lengthXorMask);

		if (!newMask)
			goto fail;

		pointer_color->xorMaskData = newMask;
		Stream_Read(s, pointer_color->xorMaskData, pointer_color->lengthXorMask);
	}

	if (pointer_color->lengthAndMask > 0)
	{
		 
		if (Stream_GetRemainingLength(s) < pointer_color->lengthAndMask)
			goto fail;

		scanlineSize = ((7 + pointer_color->width) / 8);
		scanlineSize = ((1 + scanlineSize) / 2) * 2;

		if (scanlineSize * pointer_color->height != pointer_color->lengthAndMask)
		{
			WLog_ERR(TAG,  "invalid lengthAndMask: %"PRIu32" instead of %"PRIu32"",
			         pointer_color->lengthAndMask, scanlineSize * pointer_color->height);
			goto fail;
		}

		newMask = realloc(pointer_color->andMaskData, pointer_color->lengthAndMask);

		if (!newMask)
			goto fail;

		pointer_color->andMaskData = newMask;
		Stream_Read(s, pointer_color->andMaskData, pointer_color->lengthAndMask);
	}

	if (Stream_GetRemainingLength(s) > 0)
		Stream_Seek_UINT8(s);  

	return TRUE;
fail:
	return FALSE;
}

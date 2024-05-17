BOOL license_read_binary_blob(wStream* s, LICENSE_BLOB* blob)
{
	UINT16 wBlobType;

	if (Stream_GetRemainingLength(s) < 4)
		return FALSE;

	Stream_Read_UINT16(s, wBlobType);  
	Stream_Read_UINT16(s, blob->length);  

	if (Stream_GetRemainingLength(s) < blob->length)
		return FALSE;

	 
	if ((blob->type != BB_ANY_BLOB) && (blob->length == 0))
		return TRUE;

	if ((blob->type != wBlobType) && (blob->type != BB_ANY_BLOB))
	{
		fprintf(stderr, "license binary blob type (%x) does not match expected type (%x).\n", wBlobType, blob->type);
	}

	blob->type = wBlobType;
	blob->data = (BYTE*) malloc(blob->length);

	Stream_Read(s, blob->data, blob->length);  
	return TRUE;
}
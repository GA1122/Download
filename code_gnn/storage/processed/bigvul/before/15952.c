static XMP_Uns32 GatherInt ( const char * strPtr, size_t count )
{
	XMP_Uns32 value = 0;
	const char * strEnd = strPtr + count;

	while ( strPtr < strEnd ) {
		char ch = *strPtr;
		if ( (ch < '0') || (ch > '9') ) break;
		value = value*10 + (ch - '0');
		++strPtr;
	}

	return value;

}	 

static bool DecodeRational ( const char * ratio, XMP_Uns32 * num, XMP_Uns32 * denom ) {

	unsigned long locNum, locDenom;
	char nextChar;	 

	int items = sscanf ( ratio, "%lu/%lu%c", &locNum, &locDenom, &nextChar );	 

	if ( items != 2 ) {
		if ( items != 1 ) return false;
		locDenom = 1;	 
	}
	
	*num = (XMP_Uns32)locNum;
	*denom = (XMP_Uns32)locDenom;
	return true;

}	 

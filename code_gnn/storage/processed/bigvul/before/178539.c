  static size_t TrimTrailingSpaces ( char * firstChar, size_t origLen )
  {
	if ( origLen == 0 ) return 0;
// 	if ( !firstChar || origLen == 0 ) return 0;
  
  	char * lastChar  = firstChar + origLen - 1;
  	if ( (*lastChar != ' ') && (*lastChar != 0) ) return origLen;	 
 	
 	while ( (firstChar <= lastChar) && ((*lastChar == ' ') || (*lastChar == 0)) ) --lastChar;
 	
 	XMP_Assert ( (lastChar == firstChar-1) ||
 				 ((lastChar >= firstChar) && (*lastChar != ' ') && (*lastChar != 0)) );
 	
 	size_t newLen = (size_t)((lastChar+1) - firstChar);
 	XMP_Assert ( newLen <= origLen );
 
 	if ( newLen < origLen ) {
 		++lastChar;
 		*lastChar = 0;
 	}
 
 	return newLen;
 
 }	 
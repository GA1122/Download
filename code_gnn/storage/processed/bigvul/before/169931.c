xsltNumberFormatDecimal(xmlBufferPtr buffer,
			double number,
			int digit_zero,
			int width,
			int digitsPerGroup,
			int groupingCharacter,
			int groupingCharacterLen)
{
     
    xmlChar temp_string[500];
    xmlChar *pointer;
    xmlChar temp_char[6];
    int i;
    int val;
    int len;

     
    pointer = &temp_string[sizeof(temp_string)] - 1;	 
    *pointer = 0;
    i = 0;
    while (pointer > temp_string) {
	if ((i >= width) && (fabs(number) < 1.0))
	    break;  
	if ((i > 0) && (groupingCharacter != 0) &&
	    (digitsPerGroup > 0) &&
	    ((i % digitsPerGroup) == 0)) {
	    if (pointer - groupingCharacterLen < temp_string) {
	        i = -1;		 
		break;
	    }
	    pointer -= groupingCharacterLen;
	    xmlCopyCharMultiByte(pointer, groupingCharacter);
	}

	val = digit_zero + (int)fmod(number, 10.0);
	if (val < 0x80) {			 
	    if (pointer <= temp_string) {	 
	        i = -1;
		break;
	    }
	    *(--pointer) = val;
	}
	else {
	 
	    len = xmlCopyCharMultiByte(temp_char, val);
	    if ( (pointer - len) < temp_string ) {
	        i = -1;
		break;
	    }
	    pointer -= len;
	    memcpy(pointer, temp_char, len);
	}
	number /= 10.0;
	++i;
    }
    if (i < 0)
        xsltGenericError(xsltGenericErrorContext,
		"xsltNumberFormatDecimal: Internal buffer size exceeded");
    xmlBufferCat(buffer, pointer);
}

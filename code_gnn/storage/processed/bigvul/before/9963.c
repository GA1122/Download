void PSIR_FileWriter::SetImgRsrc ( XMP_Uns16 id, const void* clientPtr, XMP_Uns32 length )
{
	InternalRsrcInfo* rsrcPtr = 0;
	InternalRsrcMap::iterator rsrcPos = this->imgRsrcs.find ( id );

	if ( rsrcPos == this->imgRsrcs.end() ) {

		InternalRsrcMap::value_type mapValue ( id, InternalRsrcInfo ( id, length, this->fileParsed ) );
		rsrcPos = this->imgRsrcs.insert ( rsrcPos, mapValue );
		rsrcPtr = &rsrcPos->second;

	} else {

		rsrcPtr = &rsrcPos->second;

		if ( (length == rsrcPtr->dataLen) &&
			 (memcmp ( rsrcPtr->dataPtr, clientPtr, length ) == 0) ) {
			return;
		}

		rsrcPtr->FreeData();		 
		rsrcPtr->dataLen = length;	 

	}

	rsrcPtr->changed = true;
	rsrcPtr->dataPtr = malloc ( length );
	if ( rsrcPtr->dataPtr == 0 ) XMP_Throw ( "Out of memory", kXMPErr_NoMemory );
	memcpy ( rsrcPtr->dataPtr, clientPtr, length );	 

	this->changed = true;

}	 
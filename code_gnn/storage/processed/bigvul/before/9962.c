void PSIR_FileWriter::ParseMemoryResources ( const void* data, XMP_Uns32 length, bool copyData   )
{
	this->DeleteExistingInfo();
	this->memParsed = true;
	if ( length == 0 ) return;


	if ( ! copyData ) {
		this->memContent = (XMP_Uns8*) data;
		XMP_Assert ( ! this->ownedContent );
	} else {
		if ( length > 100*1024*1024 ) XMP_Throw ( "Outrageous length for memory-based PSIR", kXMPErr_BadPSIR );
		this->memContent = (XMP_Uns8*) malloc ( length );
		if ( this->memContent == 0 ) XMP_Throw ( "Out of memory", kXMPErr_NoMemory );
		memcpy ( this->memContent, data, length );	 
		this->ownedContent = true;
	}
	this->memLength = length;

	

	XMP_Uns8* psirPtr   = this->memContent;
	XMP_Uns8* psirEnd   = psirPtr + length;
	XMP_Uns8* psirLimit = psirEnd - kMinImgRsrcSize;

	while ( psirPtr <= psirLimit ) {

		XMP_Uns8* origin = psirPtr;	 
		XMP_Uns32 type = GetUns32BE(psirPtr);
		XMP_Uns16 id = GetUns16BE(psirPtr+4);
		psirPtr += 6;	 

		XMP_Uns8* namePtr = psirPtr;
		XMP_Uns16 nameLen = namePtr[0];			 
		psirPtr += ((nameLen + 2) & 0xFFFE);	 

		if ( psirPtr > psirEnd-4 ) break;	 

		XMP_Uns32 dataLen = GetUns32BE(psirPtr);
		psirPtr += 4;	 

		XMP_Uns32 dataOffset = (XMP_Uns32) ( psirPtr - this->memContent );
		XMP_Uns8* nextRsrc   = psirPtr + ((dataLen + 1) & 0xFFFFFFFEUL);	 

		if ( (dataLen > length) || (psirPtr > psirEnd-dataLen) ) break;	 

		if ( type != k8BIM ) {

			XMP_Uns32 rsrcOffset = XMP_Uns32( origin - this->memContent );
			XMP_Uns32 rsrcLength = XMP_Uns32( nextRsrc - origin );	 
			XMP_Assert ( (rsrcLength & 1) == 0 );
			this->otherRsrcs.push_back ( OtherRsrcInfo ( rsrcOffset, rsrcLength ) );

		} else {

			InternalRsrcInfo newInfo ( id, dataLen, kIsMemoryBased );
			newInfo.dataPtr = psirPtr;
			newInfo.origOffset = dataOffset;
			if ( nameLen != 0 ) newInfo.rsrcName = namePtr;

			InternalRsrcMap::iterator rsrcPos = this->imgRsrcs.find ( id );
			if ( rsrcPos == this->imgRsrcs.end() ) {
				this->imgRsrcs.insert ( rsrcPos, InternalRsrcMap::value_type ( id, newInfo ) );
			} else if ( (rsrcPos->second.dataLen == 0) && (newInfo.dataLen != 0) ) {
				rsrcPos->second = newInfo;
			}

		}

		psirPtr = nextRsrc;

	}

}	 
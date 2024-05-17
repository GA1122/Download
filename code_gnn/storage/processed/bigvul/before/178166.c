 void PSIR_FileWriter::ParseFileResources ( XMP_IO* fileRef, XMP_Uns32 length )
 {
 	
 	
 
 	static const size_t kMinPSIRSize = 12;	 
 	
 	this->DeleteExistingInfo();
 	this->fileParsed = true;
 	if ( length == 0 ) return;
 
 	XMP_Int64 psirOrigin = fileRef->Offset();	 
 	XMP_Int64 fileEnd = psirOrigin + length;
 
 	char nameBuffer [260];	 
 
 	while ( fileRef->Offset() < fileEnd ) {
 
 		if ( ! XIO::CheckFileSpace ( fileRef, kMinPSIRSize ) ) break;	 
 
 		XMP_Int64 thisRsrcPos = fileRef->Offset();
 
 		XMP_Uns32 type = XIO::ReadUns32_BE ( fileRef );
 		XMP_Uns16 id   = XIO::ReadUns16_BE ( fileRef );
 
 		XMP_Uns8 nameLen = XIO::ReadUns8 ( fileRef );	 
 		XMP_Uns16 paddedLen = (nameLen + 2) & 0xFFFE;	 
 		if ( ! XIO::CheckFileSpace ( fileRef, paddedLen+4 ) ) break;	 
 
 		nameBuffer[0] = nameLen;
 		fileRef->ReadAll ( &nameBuffer[1], paddedLen-1 );	 
  
  		XMP_Uns32 dataLen   = XIO::ReadUns32_BE ( fileRef );
  		XMP_Uns32 dataTotal = ((dataLen + 1) & 0xFFFFFFFEUL);	 
// 		 
// 		 
// 		 
// 		if (dataTotal < dataLen) {
// 			break;
// 		}
  		if ( ! XIO::CheckFileSpace ( fileRef, dataTotal ) ) break;	 
  
  		XMP_Int64 thisDataPos = fileRef->Offset();
 			continue;
 		}
 
 		InternalRsrcInfo newInfo ( id, dataLen, kIsFileBased );
 		InternalRsrcMap::iterator rsrcPos = this->imgRsrcs.find ( id );
 		if ( rsrcPos == this->imgRsrcs.end() ) {
 			rsrcPos = this->imgRsrcs.insert ( rsrcPos, InternalRsrcMap::value_type ( id, newInfo ) );
 		} else if ( (rsrcPos->second.dataLen == 0) && (newInfo.dataLen != 0) ) {
 			rsrcPos->second = newInfo;
 		} else {
 			fileRef->Seek ( nextRsrcPos, kXMP_SeekFromStart );
 			continue;
 		}
 		InternalRsrcInfo* rsrcPtr = &rsrcPos->second;
 
 		rsrcPtr->origOffset = (XMP_Uns32)thisDataPos;
 
 		if ( nameLen > 0 ) {
 			rsrcPtr->rsrcName = (XMP_Uns8*) malloc ( paddedLen );
 			if ( rsrcPtr->rsrcName == 0 ) XMP_Throw ( "Out of memory", kXMPErr_NoMemory );
 			memcpy ( (void*)rsrcPtr->rsrcName, nameBuffer, paddedLen );	 
 		}
 
 		if ( ! IsMetadataImgRsrc ( id ) ) {
 			fileRef->Seek ( nextRsrcPos, kXMP_SeekFromStart );
 			continue;
 		}
 
 		rsrcPtr->dataPtr = malloc ( dataTotal );	 
 		if ( rsrcPtr->dataPtr == 0 ) XMP_Throw ( "Out of memory", kXMPErr_NoMemory );
 		fileRef->ReadAll ( (void*)rsrcPtr->dataPtr, dataTotal );
 
 	}
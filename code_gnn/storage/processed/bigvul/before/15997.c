bool TIFF_MemoryReader::GetIFD ( XMP_Uns8 ifd, TagInfoMap* ifdMap ) const
{
	if ( ifd > kTIFF_LastRealIFD ) XMP_Throw ( "Invalid IFD requested", kXMPErr_InternalFailure );
	const TweakedIFDInfo* thisIFD = &containedIFDs[ifd];

	if ( ifdMap != 0 ) ifdMap->clear();
	if ( thisIFD->count == 0 ) return false;

	if ( ifdMap != 0 ) {

		for ( size_t i = 0; i < thisIFD->count; ++i ) {

			TweakedIFDEntry* thisTag = &(thisIFD->entries[i]);
			if ( (thisTag->type < kTIFF_ByteType) || (thisTag->type > kTIFF_LastType) ) continue;	 

			TagInfo info ( thisTag->id, thisTag->type, 0, 0, GetUns32AsIs(&thisTag->bytes)  );
			info.count = info.dataLen / (XMP_Uns32)kTIFF_TypeSizes[info.type];
			info.dataPtr = this->GetDataPtr ( thisTag );

			(*ifdMap)[info.id] = info;

		}

	}

	return true;

}	 

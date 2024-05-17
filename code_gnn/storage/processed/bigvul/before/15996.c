const TIFF_MemoryReader::TweakedIFDEntry* TIFF_MemoryReader::FindTagInIFD ( XMP_Uns8 ifd, XMP_Uns16 id ) const
{
	if ( ifd == kTIFF_KnownIFD ) {
	}

	if ( ifd > kTIFF_LastRealIFD ) XMP_Throw ( "Invalid IFD requested", kXMPErr_InternalFailure );
	const TweakedIFDInfo* thisIFD = &containedIFDs[ifd];

	if ( thisIFD->count == 0 ) return 0;

	XMP_Uns32 spanLength = thisIFD->count;
	const TweakedIFDEntry* spanBegin = &(thisIFD->entries[0]);

	while ( spanLength > 1 ) {

		XMP_Uns32 halfLength = spanLength >> 1;	 
		const TweakedIFDEntry* spanMiddle = spanBegin + halfLength;


		XMP_Uns16 middleID = GetUns16AsIs ( &spanMiddle->id );
		if ( middleID == id ) {
			spanBegin = spanMiddle;
			break;
		} else if ( middleID > id ) {
			spanLength = halfLength;	 
		} else {
			spanBegin = spanMiddle;	 
			spanLength -= halfLength;
		}

	}

	if ( GetUns16AsIs(&spanBegin->id) != id ) spanBegin = 0;
	return spanBegin;

}	 

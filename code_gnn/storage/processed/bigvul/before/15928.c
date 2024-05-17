bool ASF_Support::ReadBuffer ( XMP_IO* fileRef, XMP_Uns64 & pos, XMP_Uns64 len, char * outBuffer )
{
	try {

		if ( (fileRef == 0) || (outBuffer == 0) ) return false;
	
		fileRef->Seek ( pos, kXMP_SeekFromStart  );
		long bytesRead = fileRef->ReadAll ( outBuffer, XMP_Int32(len) );
		if ( XMP_Uns32 ( bytesRead ) != len ) return false;

		return true;

	} catch ( ... ) {}

	return false;

}

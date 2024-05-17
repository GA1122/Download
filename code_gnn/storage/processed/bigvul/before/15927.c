long ASF_Support::OpenASF ( XMP_IO* fileRef, ObjectState & inOutObjectState )
{
	XMP_Uns64 pos = 0;
	XMP_Uns64 len;

	try {
		pos = fileRef->Rewind();
	} catch ( ... ) {}

	if ( pos != 0 ) return 0;

	while ( ReadObject ( fileRef, inOutObjectState, &len, pos) ) {}

	return inOutObjectState.objects.size();

}

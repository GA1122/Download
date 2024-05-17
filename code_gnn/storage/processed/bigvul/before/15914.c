bool ASF_Support::CopyObject ( XMP_IO* sourceRef, XMP_IO* destRef, const ObjectData& object )
{
	try {
		sourceRef->Seek ( object.pos, kXMP_SeekFromStart );
		XIO::Copy ( sourceRef, destRef, object.len );
	} catch ( ... ) {
		return false;
	}

	return true;

}

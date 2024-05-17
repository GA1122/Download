bool ASF_Support::ReadHeaderExtensionObject ( XMP_IO* fileRef, ObjectState& inOutObjectState, const XMP_Uns64& _pos, const ASF_ObjectBase& _objectBase )
{
	if ( ! IsEqualGUID ( ASF_Header_Extension_Object, _objectBase.guid) || (! legacyManager ) ) return false;

	try {

		const XMP_Uns64 offset = 46;
		XMP_Uns64 read = 0;
		XMP_Uns64 data = (_objectBase.size - offset);
		XMP_Uns64 pos = (_pos + offset);

		ASF_ObjectBase objectBase;

		while ( read < data ) {

			fileRef->Seek ( pos, kXMP_SeekFromStart );
			if ( kASF_ObjectBaseLen != fileRef->Read ( &objectBase, kASF_ObjectBaseLen, true ) ) break;

			objectBase.size = GetUns64LE ( &objectBase.size );

			if ( IsEqualGUID ( ASF_Padding_Object, objectBase.guid ) ) {
				legacyManager->SetPadding ( legacyManager->GetPadding() + (objectBase.size - 24) );
			}

			pos += objectBase.size;
			read += objectBase.size;

		}

	} catch ( ... ) {

		return false;

	}

	return true;

}

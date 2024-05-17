bool ASF_Support::CreatePaddingObject ( std::string* header, const XMP_Uns64 size )
{
	if ( ( ! header) || (size < 24) ) return false;

	ASF_ObjectBase newObjectBase;

	newObjectBase.guid = ASF_Padding_Object;
	newObjectBase.size = MakeUns64LE ( size );

	header->append ( (const char*)&newObjectBase, kASF_ObjectBaseLen );

	header->append ( XMP_Uns32 ( size - 24 ), '\0' );

	return true;

}

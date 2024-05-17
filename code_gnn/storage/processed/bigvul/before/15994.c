static void TrimTrailingSpaces ( TIFF_Manager::TagInfo * info )
{
	info->dataLen = (XMP_Uns32) TrimTrailingSpaces ( (char*)info->dataPtr, (size_t)info->dataLen );
}

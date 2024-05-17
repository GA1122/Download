static void TrimTrailingSpaces ( std::string * stdstr )
{
	size_t origLen = stdstr->size();
	size_t newLen = TrimTrailingSpaces ( (char*)stdstr->c_str(), origLen );
	if ( newLen != origLen ) stdstr->erase ( newLen );
}

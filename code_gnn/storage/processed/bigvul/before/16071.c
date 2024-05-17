void Chunk::changesAndSize( RIFF_MetaHandler* handler )
{
	hasChange = false;  
	this->newSize = this->oldSize;
}

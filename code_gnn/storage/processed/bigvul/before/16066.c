void ValueChunk::SetValue( std::string value, bool optionalNUL   )
{
	this->newValue.assign( value );
	if ( (! optionalNUL) || ((value.size() & 1) == 1) ) {
		this->newValue.append( 1, '\0' );  
	}
	this->newSize = this->newValue.size() + 8;
}

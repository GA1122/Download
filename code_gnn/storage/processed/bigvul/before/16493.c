config_insert( const char* attrName, const char* attrValue )
{
	if( ! (attrName && attrValue) ) {
		return;
	}
	insert( attrName, attrValue, ConfigTab, TABLESIZE );
}

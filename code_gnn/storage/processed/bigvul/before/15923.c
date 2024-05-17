int IsEqualGUID ( const GUID& guid1, const GUID& guid2 )
{
	return (memcmp ( &guid1, &guid2, sizeof(GUID) ) == 0);
}

static u32 xdr_padsize(u32 len)
{
 	return (len & 3) ? (4 - (len & 3)) : 0;
 }
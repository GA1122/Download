int Get32s(void * Long)
{
 if (MotorolaOrder){
 return ((( char *)Long)[0] << 24) | (((uchar *)Long)[1] << 16)
 | (((uchar *)Long)[2] << 8 ) | (((uchar *)Long)[3] << 0 );
 }else{
 return ((( char *)Long)[3] << 24) | (((uchar *)Long)[2] << 16)
 | (((uchar *)Long)[1] << 8 ) | (((uchar *)Long)[0] << 0 );
 }
}

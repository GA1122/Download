static void Put16u(void * Short, unsigned short PutValue)
{
 if (MotorolaOrder){
 ((uchar *)Short)[0] = (uchar)(PutValue>>8);
 ((uchar *)Short)[1] = (uchar)PutValue;
 }else{
 ((uchar *)Short)[0] = (uchar)PutValue;
 ((uchar *)Short)[1] = (uchar)(PutValue>>8);
 }
}

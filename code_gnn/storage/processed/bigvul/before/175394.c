static ogg_int32_t _float32_unpack(long val,int *point){
 long   mant=val&0x1fffff;
 int    sign=val&0x80000000;

 *point=((val&0x7fe00000L)>>21)-788;

 if(mant){
 while(!(mant&0x40000000)){
      mant<<=1;
 *point-=1;
 }
 if(sign)mant= -mant;
 }else{
 *point=-9999;
 }
 return mant;
}

static int findClosestEntry(UCHAR goalSb, UCHAR *v_k_master, UCHAR numMaster, UCHAR direction)
{
 int index;

 if( goalSb <= v_k_master[0] )
 return v_k_master[0];

 if( goalSb >= v_k_master[numMaster] )
 return v_k_master[numMaster];

 if(direction) {
    index = 0;
 while( v_k_master[index] < goalSb ) {
      index++;
 }
 } else {
    index = numMaster;
 while( v_k_master[index] > goalSb ) {
      index--;
 }
 }

 return v_k_master[index];
}

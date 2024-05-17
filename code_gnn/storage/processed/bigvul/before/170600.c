void From2iToMono_32( const LVM_INT32 *src,
                            LVM_INT32 *dst,
                            LVM_INT16 n)
{
   LVM_INT16 ii;
   LVM_INT32 Temp;

 for (ii = n; ii != 0; ii--)
 {
 Temp = (*src>>1);
       src++;

 Temp +=(*src>>1);
       src++;

 *dst = Temp;
       dst++;
 }

 return;
}

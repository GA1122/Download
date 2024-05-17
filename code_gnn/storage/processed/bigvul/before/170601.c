void MonoTo2I_32( const LVM_INT32  *src,
                        LVM_INT32  *dst,
                        LVM_INT16 n)
{
   LVM_INT16 ii;
   src += (n-1);
   dst += ((n*2)-1);

 for (ii = n; ii != 0; ii--)
 {
 *dst = *src;
       dst--;

 *dst = *src;
       dst--;
       src--;
 }

 return;
}

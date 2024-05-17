int32 PVLocateFrameHeader(uint8 *ptr, int32 size)
{
 int count = 0;
 int32 i = size;

 if (size < 1)
 {
 return 0;
 }
 while (i--)
 {
 if ((count > 1) && (*ptr == 0x01))
 {
            i += 2;
 break;
 }

 if (*ptr++)
            count = 0;
 else
            count++;
 }
 return (size - (i + 1));
}

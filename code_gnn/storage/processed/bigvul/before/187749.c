  main(void)
  {
  unsigned int i;
  unsigned char buf[MAX_LENGTH];
  unsigned long crc;
  unsigned char c;
  int inchar;
 
 
   
     for (i=8; i; i--)
     {
      c=GETBREAK;
//       GETBREAK;
        putchar(c);
     }
  
if (inchar != EOF)
// if (inchar == c)  
  for (;;)
   {
      
     unsigned long length;  
   c=GETBREAK; buf[0] = c; length  = c; length <<= 8;
   c=GETBREAK; buf[1] = c; length += c; length <<= 8;
   c=GETBREAK; buf[2] = c; length += c; length <<= 8;
   c=GETBREAK; buf[3] = c; length += c;
//    GETBREAK; buf[0] = c; length  = c; length <<= 8;
//    GETBREAK; buf[1] = c; length += c; length <<= 8;
//    GETBREAK; buf[2] = c; length += c; length <<= 8;
//    GETBREAK; buf[3] = c; length += c;
  
      
   c=GETBREAK; buf[4] = c;
   c=GETBREAK; buf[5] = c;
   c=GETBREAK; buf[6] = c;
   c=GETBREAK; buf[7] = c;
//    GETBREAK; buf[4] = c;
//    GETBREAK; buf[5] = c;
//    GETBREAK; buf[6] = c;
//    GETBREAK; buf[7] = c;
  
  
      
  if (buf[4] == 105 && buf[5] == 84 && buf[6] == 88 && buf[7] == 116)
  {
  if (length >= MAX_LENGTH-12)
  break;  
 
   
       crc = crc32(0, Z_NULL, 0);
 
 
         
        for (i=8; i < length + 12; i++)
        {
         c=GETBREAK; buf[i] = c;
//          GETBREAK; buf[i] = c;
        }
  
//       if (inchar != c)  
//          break;
// 
         
        crc = crc32(crc, buf+4, (uInt)length+4);
  
        for (;;)
        {
           
        if (((crc >> 24) & 0xff) == buf[length+8] &&
            ((crc >> 16) & 0xff) == buf[length+9] &&
            ((crc >>  8) & 0xff) == buf[length+10] &&
            ((crc      ) & 0xff) == buf[length+11])
//         if (((crc >> 24) & 0xffU) == buf[length+8] &&
//             ((crc >> 16) & 0xffU) == buf[length+9] &&
//             ((crc >>  8) & 0xffU) == buf[length+10] &&
//             ((crc      ) & 0xffU) == buf[length+11])
             break;
  
          length++;
 
 
          if (length >= MAX_LENGTH-12)
             break;
  
        c=GETBREAK;
        buf[length+11]=c;
//         GETBREAK;
//         buf[length+11] = c;
  
           
          crc = crc32(crc, buf+7+length, 1);
        }
  
//       if (inchar != c)  
//          break;
// 
         
      buf[0] = (unsigned char)((length << 24) & 0xff);
      buf[1] = (unsigned char)((length << 16) & 0xff);
      buf[2] = (unsigned char)((length <<  8) & 0xff);
      buf[3] = (unsigned char)((length      ) & 0xff);
//       buf[0] = (unsigned char)((length >> 24) & 0xffU);
//       buf[1] = (unsigned char)((length >> 16) & 0xffU);
//       buf[2] = (unsigned char)((length >>  8) & 0xffU);
//       buf[3] = (unsigned char)((length      ) & 0xffU);
  
         
        for (i=0; i<length+12; i++)
          putchar(buf[i]);
  }
 
  
     else
     {
//       if (inchar != c)  
//          break;
// 
         
        for (i=0; i<8; i++)
           putchar(buf[i]);
 
 
         
        for (i=8; i< length+12; i++)
        {
         c=GETBREAK;
//          GETBREAK;
           putchar(c);
        }
  
      if (inchar == EOF)
//       if (inchar != c)  
        {
           break;
        }
 
   
  if (buf[4] == 73 && buf[5] == 69 && buf[6] == 78 && buf[7] == 68)
 
           break;
     }
  
   if (inchar == EOF)
//    if (inchar != c)  
        break;
  
     if (buf[4] == 73 && buf[5] == 69 && buf[6] == 78 && buf[7] == 68)
  break;
  }
 
  return 0;
 }
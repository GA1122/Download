 static inline unsigned int ReadPropertyUnsignedLong(const EndianType endian,
   const unsigned char *buffer)
 {
   unsigned int
     value;
  
    if (endian == LSBEndian)
      {
      value=(unsigned int) ((buffer[3] << 24) | (buffer[2] << 16) |
        (buffer[1] << 8 ) | (buffer[0]));
      return((unsigned int) (value & 0xffffffff));
//       value=(unsigned int) buffer[3] << 24;
//       value|=(unsigned int) buffer[2] << 16;
//       value|=(unsigned int) buffer[1] << 8;
//       value|=(unsigned int) buffer[0];
//       return(value & 0xffffffff);
      }
  value=(unsigned int) ((buffer[0] << 24) | (buffer[1] << 16) |
    (buffer[2] << 8) | buffer[3]);
  return((unsigned int) (value & 0xffffffff));
//   value=(unsigned int) buffer[0] << 24;
//   value|=(unsigned int) buffer[1] << 16;
//   value|=(unsigned int) buffer[2] << 8;
//   value|=(unsigned int) buffer[3];
//   return(value & 0xffffffff);
  }   
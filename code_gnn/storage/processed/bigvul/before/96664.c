MagickExport ssize_t WriteBlobSignedLong(Image *image,const signed int value)
{
  union
  {
    unsigned int
      unsigned_value;

    signed int
      signed_value;
  } quantum;

  unsigned char
    buffer[4];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  quantum.signed_value=value;
  if (image->endian == LSBEndian)
    {
      buffer[0]=(unsigned char) quantum.unsigned_value;
      buffer[1]=(unsigned char) (quantum.unsigned_value >> 8);
      buffer[2]=(unsigned char) (quantum.unsigned_value >> 16);
      buffer[3]=(unsigned char) (quantum.unsigned_value >> 24);
      return(WriteBlobStream(image,4,buffer));
    }
  buffer[0]=(unsigned char) (quantum.unsigned_value >> 24);
  buffer[1]=(unsigned char) (quantum.unsigned_value >> 16);
  buffer[2]=(unsigned char) (quantum.unsigned_value >> 8);
  buffer[3]=(unsigned char) quantum.unsigned_value;
  return(WriteBlobStream(image,4,buffer));
}
static int UnpackWPGRaster(Image *image,int bpp)
{
  int
    x,
    y,
    i;

  unsigned char
    bbuf,
    *BImgBuff,
    RunCount;

  ssize_t
    ldblk;

  x=0;
  y=0;

  ldblk=(ssize_t) ((bpp*image->columns+7)/8);
  BImgBuff=(unsigned char *) AcquireQuantumMemory((size_t) ldblk,
    8*sizeof(*BImgBuff));
  if(BImgBuff==NULL) return(-2);

  while(y<(ssize_t) image->rows)
  {
    int
      c;

    c=ReadBlobByte(image);
    if (c ==  EOF)
      break;
    bbuf=(unsigned char) c;
      RunCount=bbuf & 0x7F;
      if(bbuf & 0x80)
        {
          if(RunCount)   
            {
              bbuf=ReadBlobByte(image);
              for(i=0;i<(int) RunCount;i++) InsertByte(bbuf);
            }
          else {   
            c=ReadBlobByte(image);
            if (c < 0)
              break;
            RunCount=(unsigned char) c;
            for(i=0;i<(int) RunCount;i++) InsertByte(0xFF);
          }
        }
      else {
        if(RunCount)    
          {
            for(i=0;i < (int) RunCount;i++)
              {
                bbuf=ReadBlobByte(image);
                InsertByte(bbuf);
              }
          }
        else {   
          c=ReadBlobByte(image);
          if (c < 0)
            break;
          RunCount=(unsigned char) c;
          if(x) {     
             
            BImgBuff=(unsigned char *) RelinquishMagickMemory(BImgBuff);
            return(-3);
          }
          for(i=0;i < (int) RunCount;i++)
            {
              x=0;
              y++;     
              if(y<2) continue;
              if(y>(ssize_t) image->rows)
                {
                  BImgBuff=(unsigned char *) RelinquishMagickMemory(BImgBuff);
                  return(-4);
                }
              InsertRow(BImgBuff,y-1,image,bpp);
            }
        }
      }
    }
  BImgBuff=(unsigned char *) RelinquishMagickMemory(BImgBuff);
  return(y < (ssize_t) image->rows ? -5 : 0);
}

 int main(int argc, char **argv)
 {
 
     u8 *byteStrmStart;
     u8 *byteStrm;
     u32 strmLen;
     u32 picSize;
     H264SwDecInst decInst;
     H264SwDecRet ret;
     H264SwDecInput decInput;
     H264SwDecOutput decOutput;
     H264SwDecPicture decPicture;
     H264SwDecInfo decInfo;
     u32 picNumber;
 
  FILE *finput;
  FILE *foutput;
 
   
  if (argc < 2)
  {
         printf( "Usage: %s file.h264\n", argv[0]);
  return -1;
  }
 
   
     foutput = fopen("out.yuv", "wb");
  if (foutput == NULL)
  {
         printf("UNABLE TO OPEN OUTPUT FILE\n");
  return -1;
  }
 
   
     finput = fopen(argv[argc-1], "rb");
  if (finput == NULL)
  {
         printf("UNABLE TO OPEN INPUT FILE\n");
  return -1;
  }
 
   
     fseek(finput, 0L, SEEK_END);
     strmLen = (u32)ftell(finput);
 
      rewind(finput);
  
       
    byteStrm = byteStrmStart = (u8 *)H264SwDecMalloc(sizeof(u8)*strmLen);
//     byteStrm = byteStrmStart = (u8 *)H264SwDecMalloc(sizeof(u8), strmLen);
      if (byteStrm == NULL)
      {
          printf("UNABLE TO ALLOCATE MEMORY\n");
  return -1;
  }
 
   
     fread(byteStrm, sizeof(u8), strmLen, finput);
     fclose(finput);
 
   
     ret = H264SwDecInit(&decInst, 0);
  if (ret != H264SWDEC_OK)
  {
         printf("DECODER INITIALIZATION FAILED\n");
  return -1;
  }
 
   
     decInput.pStream = byteStrmStart;
     decInput.dataLen = strmLen;
     decInput.intraConcealmentMethod = 0;
 
     picNumber = 0;
 
   
 
   
  do
  {
   
         ret = H264SwDecDecode(decInst, &decInput, &decOutput);
 
  switch(ret)
  {
 
  case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
 
   
                 ret = H264SwDecGetInfo(decInst, &decInfo);
  if (ret != H264SWDEC_OK)
  return -1;
 
   
                 picSize = decInfo.picWidth * decInfo.picHeight;
   
                 picSize = (3 * picSize)/2;
   
 
                 printf("Width %d Height %d\n",
                     decInfo.picWidth, decInfo.picHeight);
 
   
                 decInput.dataLen -=
  (u32)(decOutput.pStrmCurrPos - decInput.pStream);
                 decInput.pStream = decOutput.pStrmCurrPos;
  break;
 
  case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
  case H264SWDEC_PIC_RDY:
 
   
                 decInput.dataLen -=
  (u32)(decOutput.pStrmCurrPos - decInput.pStream);
                 decInput.pStream = decOutput.pStrmCurrPos;
 
   
  while (H264SwDecNextPicture(decInst, &decPicture, 0) ==
                     H264SWDEC_PIC_RDY) { picNumber++;
 
                     printf("PIC %d, type %s, concealed %d\n", picNumber,
                         decPicture.isIdrPicture ? "IDR" : "NON-IDR",
                         decPicture.nbrOfErrMBs);
                     fflush(stdout);
 
   
 
   
  WriteOutput(foutput, (u8*)decPicture.pOutputPicture,
                         picSize);
  }
 
  break;
 
  case H264SWDEC_EVALUATION_LIMIT_EXCEEDED:
   
                 printf("EVALUATION LIMIT REACHED\n");
  goto end;
 
  default:
                 printf("UNRECOVERABLE ERROR\n");
  return -1;
  }
   
  } while (decInput.dataLen > 0);
 
 end:
 
   
  while (H264SwDecNextPicture(decInst, &decPicture, 1) ==
         H264SWDEC_PIC_RDY) {
 
         picNumber++;
 
         printf("PIC %d, type %s, concealed %d\n", picNumber,
             decPicture.isIdrPicture ? "IDR" : "NON-IDR",
             decPicture.nbrOfErrMBs);
         fflush(stdout);
 
   
 
   
  WriteOutput(foutput, (u8*)decPicture.pOutputPicture, picSize);
  }
 
   
 
 
   
     H264SwDecRelease(decInst);
 
   
     fclose(foutput);
 
   
     free(byteStrmStart);
 
  return 0;
 
 }
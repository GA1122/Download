int main(int argc, char **argv)
{

    u32 i, tmp;
    u32 maxNumPics = 0;
    u8 *byteStrmStart;
    u8 *imageData;
    u8 *tmpImage = NULL;
    u32 strmLen;
    u32 picSize;
    H264SwDecInst decInst;
    H264SwDecRet ret;
    H264SwDecInput decInput;
    H264SwDecOutput decOutput;
    H264SwDecPicture decPicture;
    H264SwDecInfo decInfo;
    H264SwDecApiVersion decVer;
    u32 picDecodeNumber;
    u32 picDisplayNumber;
    u32 numErrors = 0;
    u32 cropDisplay = 0;
    u32 disableOutputReordering = 0;

 FILE *finput;

 char outFileName[256] = "";

  
    decVer = H264SwDecGetAPIVersion();
    DEBUG(("H.264 Decoder API v%d.%d\n", decVer.major, decVer.minor));

  
 if ( argc > 1 && strcmp(argv[1], "-T") == 0 )
 {
        DEBUG(("%s\n", tagName));
 return 0;
 }

  
 if (argc < 2)
 {
        DEBUG((
 "Usage: %s [-Nn] [-Ooutfile] [-P] [-U] [-C] [-R] [-T] file.h264\n",
            argv[0]));
        DEBUG(("\t-Nn forces decoding to stop after n pictures\n"));
#if defined(_NO_OUT)
        DEBUG(("\t-Ooutfile output writing disabled at compile time\n"));
#else
        DEBUG(("\t-Ooutfile write output to \"outfile\" (default out_wxxxhyyy.yuv)\n"));
        DEBUG(("\t-Onone does not write output\n"));
#endif
        DEBUG(("\t-P packet-by-packet mode\n"));
        DEBUG(("\t-U NAL unit stream mode\n"));
        DEBUG(("\t-C display cropped image (default decoded image)\n"));
        DEBUG(("\t-R disable DPB output reordering\n"));
        DEBUG(("\t-T to print tag name and exit\n"));
 return 0;
 }

  
 for (i = 1; i < (u32)(argc-1); i++)
 {
 if ( strncmp(argv[i], "-N", 2) == 0 )
 {
            maxNumPics = (u32)atoi(argv[i]+2);
 }
 else if ( strncmp(argv[i], "-O", 2) == 0 )
 {
            strcpy(outFileName, argv[i]+2);
 }
 else if ( strcmp(argv[i], "-P") == 0 )
 {
            packetize = 1;
 }
 else if ( strcmp(argv[i], "-U") == 0 )
 {
            nalUnitStream = 1;
 }
 else if ( strcmp(argv[i], "-C") == 0 )
 {
            cropDisplay = 1;
 }
 else if ( strcmp(argv[i], "-R") == 0 )
 {
            disableOutputReordering = 1;
 }
 }

  
    finput = fopen(argv[argc-1],"rb");
 if (finput == NULL)
 {
        DEBUG(("UNABLE TO OPEN INPUT FILE\n"));
 return -1;
 }

  
    fseek(finput,0L,SEEK_END);
    strmLen = (u32)ftell(finput);
    rewind(finput);

  
    byteStrmStart = (u8 *)malloc(sizeof(u8)*strmLen);
 if (byteStrmStart == NULL)
 {
        DEBUG(("UNABLE TO ALLOCATE MEMORY\n"));
 return -1;
 }

  
    fread(byteStrmStart, sizeof(u8), strmLen, finput);
    fclose(finput);

  
    ret = H264SwDecInit(&decInst, disableOutputReordering);
 if (ret != H264SWDEC_OK)
 {
        DEBUG(("DECODER INITIALIZATION FAILED\n"));
        free(byteStrmStart);
 return -1;
 }

  
    streamStop = byteStrmStart + strmLen;
    decInput.pStream = byteStrmStart;
    decInput.dataLen = strmLen;
    decInput.intraConcealmentMethod = 0;

  
 if ( (tmp = NextPacket(&decInput.pStream)) != 0 )
        decInput.dataLen = tmp;

    picDecodeNumber = picDisplayNumber = 1;
  
 do
 {
  
        decInput.picId = picDecodeNumber;

  
        ret = H264SwDecDecode(decInst, &decInput, &decOutput);

 switch(ret)
 {

 case H264SWDEC_HDRS_RDY_BUFF_NOT_EMPTY:
  

                ret = H264SwDecGetInfo(decInst, &decInfo);
 if (ret != H264SWDEC_OK)
 return -1;

                DEBUG(("Profile %d\n", decInfo.profile));

                DEBUG(("Width %d Height %d\n",
                    decInfo.picWidth, decInfo.picHeight));

 if (cropDisplay && decInfo.croppingFlag)
 {
                    DEBUG(("Cropping params: (%d, %d) %dx%d\n",
                        decInfo.cropParams.cropLeftOffset,
                        decInfo.cropParams.cropTopOffset,
                        decInfo.cropParams.cropOutWidth,
                        decInfo.cropParams.cropOutHeight));

  
                    picSize = decInfo.cropParams.cropOutWidth *
                              decInfo.cropParams.cropOutHeight;
                    picSize = (3 * picSize)/2;
                    tmpImage = malloc(picSize);
 if (tmpImage == NULL)
 return -1;
 }
 else
 {
  
                    picSize = decInfo.picWidth * decInfo.picHeight;
                    picSize = (3 * picSize)/2;
 }

                DEBUG(("videoRange %d, matrixCoefficients %d\n",
                    decInfo.videoRange, decInfo.matrixCoefficients));

  
                decInput.dataLen -=
 (u32)(decOutput.pStrmCurrPos - decInput.pStream);
                decInput.pStream = decOutput.pStrmCurrPos;

  
 if (outFileName[0] == 0)
                    sprintf(outFileName, "out_w%dh%d.yuv",
                            decInfo.picWidth, decInfo.picHeight);
 break;

 case H264SWDEC_PIC_RDY_BUFF_NOT_EMPTY:
  
                decInput.dataLen -=
 (u32)(decOutput.pStrmCurrPos - decInput.pStream);
                decInput.pStream = decOutput.pStrmCurrPos;
  

 case H264SWDEC_PIC_RDY:

  

 if (ret == H264SWDEC_PIC_RDY)
                    decInput.dataLen = NextPacket(&decInput.pStream);

  
 if (maxNumPics && picDecodeNumber == maxNumPics)
                    decInput.dataLen = 0;

  
                picDecodeNumber++;

  
 while ( H264SwDecNextPicture(decInst, &decPicture, 0) ==
                        H264SWDEC_PIC_RDY )
 {
                    DEBUG(("PIC %d, type %s", picDisplayNumber,
                        decPicture.isIdrPicture ? "IDR" : "NON-IDR"));
 if (picDisplayNumber != decPicture.picId)
                        DEBUG((", decoded pic %d", decPicture.picId));
 if (decPicture.nbrOfErrMBs)
 {
                        DEBUG((", concealed %d\n", decPicture.nbrOfErrMBs));
 }
 else
                        DEBUG(("\n"));
                    fflush(stdout);

                    numErrors += decPicture.nbrOfErrMBs;

  
                    picDisplayNumber++;

  

  
                    imageData = (u8*)decPicture.pOutputPicture;
 if (cropDisplay && decInfo.croppingFlag)
 {
                        tmp = CropPicture(tmpImage, imageData,
                            decInfo.picWidth, decInfo.picHeight,
 &decInfo.cropParams);
 if (tmp)
 return -1;
 WriteOutput(outFileName, tmpImage, picSize);
 }
 else
 {
 WriteOutput(outFileName, imageData, picSize);
 }
 }

 break;

 case H264SWDEC_STRM_PROCESSED:
 case H264SWDEC_STRM_ERR:
  
                decInput.dataLen = NextPacket(&decInput.pStream);
 break;

 default:
                DEBUG(("FATAL ERROR\n"));
 return -1;

 }
  
 } while (decInput.dataLen > 0);

  
 while (H264SwDecNextPicture(decInst, &decPicture, 1) == H264SWDEC_PIC_RDY)
 {
        DEBUG(("PIC %d, type %s", picDisplayNumber,
            decPicture.isIdrPicture ? "IDR" : "NON-IDR"));
 if (picDisplayNumber != decPicture.picId)
            DEBUG((", decoded pic %d", decPicture.picId));
 if (decPicture.nbrOfErrMBs)
 {
            DEBUG((", concealed %d\n", decPicture.nbrOfErrMBs));
 }
 else
            DEBUG(("\n"));
        fflush(stdout);

        numErrors += decPicture.nbrOfErrMBs;

  
        picDisplayNumber++;

  
        imageData = (u8*)decPicture.pOutputPicture;
 if (cropDisplay && decInfo.croppingFlag)
 {
            tmp = CropPicture(tmpImage, imageData,
                decInfo.picWidth, decInfo.picHeight,
 &decInfo.cropParams);
 if (tmp)
 return -1;
 WriteOutput(outFileName, tmpImage, picSize);
 }
 else
 {
 WriteOutput(outFileName, imageData, picSize);
 }
 }

  
    H264SwDecRelease(decInst);

 if (foutput)
        fclose(foutput);

  
    free(byteStrmStart);
    free(tmpImage);

    DEBUG(("Output file: %s\n", outFileName));

    DEBUG(("DECODING DONE\n"));
 if (numErrors || picDecodeNumber == 1)
 {
        DEBUG(("ERRORS FOUND\n"));
 return 1;
 }

 return 0;
}

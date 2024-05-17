  OJPEGDecode(TIFF* tif, uint8* buf, tmsize_t cc, uint16 s)
  {
//         static const char module[]="OJPEGDecode";
  	OJPEGState* sp=(OJPEGState*)tif->tif_data;
  	(void)s;
//         if( !sp->decoder_ok )
//         {
//             TIFFErrorExt(tif->tif_clientdata,module,"Cannot decode: decoder not correctly initialized");
//             return 0;
//         }
  	if (sp->libjpeg_jpeg_query_style==0)
  	{
  		if (OJPEGDecodeRaw(tif,buf,cc)==0)
 			return(0);
 	}
 	else
 	{
 		if (OJPEGDecodeScanlines(tif,buf,cc)==0)
 			return(0);
 	}
 	return(1);
 }
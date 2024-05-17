 static Image *ReadXCFImage(const ImageInfo *image_info,ExceptionInfo *exception)
 {
   char
     magick[14];
 
   Image
     *image;
 
   int
     foundPropEnd = 0;
 
   MagickBooleanType
     status;
 
   MagickOffsetType
     offset;
 
   register ssize_t
     i;
 
   size_t
     image_type,
     length;
 
   ssize_t
     count;
 
   XCFDocInfo
     doc_info;
 
    
   assert(image_info != (const ImageInfo *) NULL);
   assert(image_info->signature == MagickSignature);
   if (image_info->debug != MagickFalse)
     (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
       image_info->filename);
   assert(exception != (ExceptionInfo *) NULL);
   assert(exception->signature == MagickSignature);
   image=AcquireImage(image_info);
   status=OpenBlob(image_info,image,ReadBinaryBlobMode,exception);
   if (status == MagickFalse)
     {
       image=DestroyImageList(image);
       return((Image *) NULL);
     }
   count=ReadBlob(image,14,(unsigned char *) magick);
   if ((count != 14) ||
       (LocaleNCompare((char *) magick,"gimp xcf",8) != 0))
     ThrowReaderException(CorruptImageError,"ImproperImageHeader");
   (void) ResetMagickMemory(&doc_info,0,sizeof(XCFDocInfo));
   doc_info.exception=exception;
   doc_info.width=ReadBlobMSBLong(image);
   doc_info.height=ReadBlobMSBLong(image);
   if ((doc_info.width > 262144) || (doc_info.height > 262144))
     ThrowReaderException(CorruptImageError,"ImproperImageHeader");
   doc_info.image_type=ReadBlobMSBLong(image);
    
   image->columns=doc_info.width;
   image->rows=doc_info.height;
   image_type=doc_info.image_type;
   doc_info.file_size=GetBlobSize(image);
   image->compression=NoCompression;
   image->depth=8;
   status=SetImageExtent(image,image->columns,image->rows);
   if (status == MagickFalse)
     {
       InheritException(exception,&image->exception);
       return(DestroyImageList(image));
     }
   if (image_type == GIMP_RGB)
     ;
   else
     if (image_type == GIMP_GRAY)
       image->colorspace=GRAYColorspace;
     else
       if (image_type == GIMP_INDEXED)
         ThrowReaderException(CoderError,"ColormapTypeNotSupported");
   (void) SetImageOpacity(image,OpaqueOpacity); 
   (void) SetImageBackgroundColor(image);
    
   while ((foundPropEnd == MagickFalse) && (EOFBlob(image) == MagickFalse))
   {
     PropType prop_type = (PropType) ReadBlobMSBLong(image);
     size_t prop_size = ReadBlobMSBLong(image);
 
     switch (prop_type)
     {
       case PROP_END:
         foundPropEnd=1;
         break;
       case PROP_COLORMAP:
       {
          
         size_t num_colours = ReadBlobMSBLong(image);
         if (DiscardBlobBytes(image,3*num_colours) == MagickFalse)
           ThrowFileException(&image->exception,CorruptImageError,
             "UnexpectedEndOfFile",image->filename);
      
         break;
       }
       case PROP_COMPRESSION:
       {
         doc_info.compression = ReadBlobByte(image);
         if ((doc_info.compression != COMPRESS_NONE) &&
             (doc_info.compression != COMPRESS_RLE) &&
             (doc_info.compression != COMPRESS_ZLIB) &&
             (doc_info.compression != COMPRESS_FRACTAL))
           ThrowReaderException(CorruptImageError,"UnrecognizedImageCompression");
       }
       break;
 
       case PROP_GUIDES:
       {
           
         if (DiscardBlobBytes(image,prop_size) == MagickFalse)
           ThrowFileException(&image->exception,CorruptImageError,
             "UnexpectedEndOfFile",image->filename);
       }
       break;
 
     case PROP_RESOLUTION:
       {
           (void) ReadBlobMSBLong(image);
           (void) ReadBlobMSBLong(image);
 
          
 
 
          
       }
       break;
 
     case PROP_TATTOO:
       {
          
           (void) ReadBlobMSBLong(image);
       }
       break;
 
     case PROP_PARASITES:
       {
          
         if (DiscardBlobBytes(image,prop_size) == MagickFalse)
           ThrowFileException(&image->exception,CorruptImageError,
             "UnexpectedEndOfFile",image->filename);
          
           }
       break;
 
     case PROP_UNIT:
       {
          
         (void) ReadBlobMSBLong(image);
       }
       break;
 
     case PROP_PATHS:
       {
        
         if (DiscardBlobBytes(image,prop_size) == MagickFalse)
           ThrowFileException(&image->exception,CorruptImageError,
             "UnexpectedEndOfFile",image->filename);
 
          
       }
       break;
 
     case PROP_USER_UNIT:
       {
         char  unit_string[1000];
          
           (void) ReadBlobMSBLong(image);
           (void) ReadBlobMSBLong(image);
         for (i=0; i<5; i++)
          (void) ReadBlobStringWithLongSize(image, unit_string,
            sizeof(unit_string));
       }
      break;
 
       default:
       {
         int buf[16];
         ssize_t amount;
 
        
       while ((prop_size > 0) && (EOFBlob(image) == MagickFalse))
       {
         amount=(ssize_t) MagickMin(16, prop_size);
         amount=(ssize_t) ReadBlob(image,(size_t) amount,(unsigned char *) &buf);
         if (!amount)
           ThrowReaderException(CorruptImageError,"CorruptImage");
         prop_size -= (size_t) MagickMin(16,(size_t) amount);
       }
     }
     break;
   }
   }
   if (foundPropEnd == MagickFalse)
     ThrowReaderException(CorruptImageError,"ImproperImageHeader");
 
   if ((image_info->ping != MagickFalse) && (image_info->number_scenes != 0))
     {
       ;  
     }
   else
     {
       int
         current_layer = 0,
         foundAllLayers = MagickFalse,
         number_layers = 0;
 
       MagickOffsetType
         oldPos=TellBlob(image);
 
       XCFLayerInfo
         *layer_info;
 
        
       do
       {
         ssize_t offset = ReadBlobMSBSignedLong(image);
         if (offset == 0)
           foundAllLayers=MagickTrue;
         else
           number_layers++;
         if (EOFBlob(image) != MagickFalse)
           {
             ThrowFileException(exception,CorruptImageError,
               "UnexpectedEndOfFile",image->filename);
             break;
           }
     } while (foundAllLayers == MagickFalse);
     doc_info.number_layers=number_layers;
     offset=SeekBlob(image,oldPos,SEEK_SET);  
     if (offset < 0)
       ThrowReaderException(CorruptImageError,"ImproperImageHeader");
      
     length=(size_t) number_layers;
     layer_info=(XCFLayerInfo *) AcquireQuantumMemory(length,
       sizeof(*layer_info));
     if (layer_info == (XCFLayerInfo *) NULL)
       ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
     (void) ResetMagickMemory(layer_info,0,number_layers*sizeof(XCFLayerInfo));
     for ( ; ; )
     {
       MagickBooleanType
         layer_ok;
 
       MagickOffsetType
         offset,
         saved_pos;
 
        
       offset=(MagickOffsetType) ReadBlobMSBLong(image);
        
       if (offset == 0)
         break;
        
       saved_pos=TellBlob(image);
        
       if (SeekBlob(image,offset,SEEK_SET) != offset)
         ThrowReaderException(ResourceLimitError,"NotEnoughPixelData");
        
       layer_ok=ReadOneLayer(image_info,image,&doc_info,
         &layer_info[current_layer],current_layer);
       if (layer_ok == MagickFalse)
         {
           int j;
 
           for (j=0; j < current_layer; j++)
             layer_info[j].image=DestroyImage(layer_info[j].image);
           layer_info=(XCFLayerInfo *) RelinquishMagickMemory(layer_info);
           ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
         }
        
       offset=SeekBlob(image, saved_pos, SEEK_SET);
       current_layer++;
     }
 #if 0
         {
          
         signed int  j;
         for (j=number_layers-1; j>=0; j--) {
            
 
           if ( layer_info[j].visible ) {  
             CompositeImage(image, OverCompositeOp, layer_info[j].image,
                      layer_info[j].offset_x, layer_info[j].offset_y );
              layer_info[j].image =DestroyImage( layer_info[j].image );
 
              
             if ( image_type == GIMP_GRAY ) {
               QuantizeInfo  qi;
               GetQuantizeInfo(&qi);
               qi.colorspace = GRAYColorspace;
               QuantizeImage( &qi, layer_info[j].image );
             }
           }
         }
       }
 #else
       {
          
         ssize_t  j;
 
          
         for (j=(long) number_layers-1; j >= 0; j--)
           AppendImageToList(&image,layer_info[j].image);
       }
 #endif
 
     layer_info=(XCFLayerInfo *) RelinquishMagickMemory(layer_info);
 
 #if 0   
     while (MagickTrue)
     {
        
       info->cp += xcf_read_int32 (info->fp, &offset, 1);
 
        
       if (offset == 0)
         break;
 
        
       saved_pos = info->cp;
 
        
       xcf_seek_pos (info, offset);
 
        
       channel = xcf_load_channel (info, gimage);
       if (channel == 0)
         goto error;
 
       num_successful_elements++;
 
        
       if (channel != gimage->selection_mask)
         gimp_image_add_channel (gimage, channel, -1);
 
        
       xcf_seek_pos (info, saved_pos);
     }
 #endif
    }
  
    (void) CloseBlob(image);
  DestroyImage(RemoveFirstImageFromList(&image));
//   if (GetNextImageInList(image) != (Image *) NULL)
//     DestroyImage(RemoveFirstImageFromList(&image));
    if (image_type == GIMP_GRAY)
      image->type=GrayscaleType;
    return(GetFirstImageInList(image));
 }
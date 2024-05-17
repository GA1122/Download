 static Image *ReadDCMImage(const ImageInfo *image_info,ExceptionInfo *exception)
 {
   char
     explicit_vr[MaxTextExtent],
     implicit_vr[MaxTextExtent],
     magick[MaxTextExtent],
     photometric[MaxTextExtent];
 
   DCMStreamInfo
     *stream_info;
 
   Image
     *image;
 
   int
     *bluemap,
     datum,
     *greenmap,
     *graymap,
     index,
     *redmap;
 
   MagickBooleanType
     explicit_file,
     explicit_retry,
     polarity,
     sequence,
     use_explicit;
 
   MagickOffsetType
     offset;
 
   Quantum
     *scale;
 
   register IndexPacket
     *indexes;
 
   register ssize_t
     i,
     x;
 
   register PixelPacket
     *q;
 
   register unsigned char
     *p;
 
   size_t
     bits_allocated,
     bytes_per_pixel,
     colors,
     depth,
     height,
     length,
     mask,
     max_value,
     number_scenes,
     quantum,
     samples_per_pixel,
     signed_data,
     significant_bits,
     status,
     width,
     window_width;
 
   ssize_t
     count,
     scene,
     window_center,
     y;
 
   unsigned char
     *data;
 
   unsigned short
     group,
     element;
 
    
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
   image->depth=8UL;
   image->endian=LSBEndian;
    
   stream_info=(DCMStreamInfo *) AcquireMagickMemory(sizeof(*stream_info));
   if (stream_info == (DCMStreamInfo *) NULL)
     ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
   (void) ResetMagickMemory(stream_info,0,sizeof(*stream_info));
   count=ReadBlob(image,128,(unsigned char *) magick);
   if (count != 128)
     ThrowReaderException(CorruptImageError,"ImproperImageHeader");
   count=ReadBlob(image,4,(unsigned char *) magick);
   if ((count != 4) || (LocaleNCompare(magick,"DICM",4) != 0))
     {
       offset=SeekBlob(image,0L,SEEK_SET);
       if (offset < 0)
         ThrowReaderException(CorruptImageError,"ImproperImageHeader");
     }
    
   (void) CopyMagickString(photometric,"MONOCHROME1 ",MaxTextExtent);
   bits_allocated=8;
   bytes_per_pixel=1;
   polarity=MagickFalse;
   data=(unsigned char *) NULL;
   depth=8;
   element=0;
   explicit_vr[2]='\0';
   explicit_file=MagickFalse;
   colors=0;
   redmap=(int *) NULL;
   greenmap=(int *) NULL;
   bluemap=(int *) NULL;
   graymap=(int *) NULL;
   height=0;
   max_value=255UL;
   mask=0xffff;
   number_scenes=1;
   samples_per_pixel=1;
   scale=(Quantum *) NULL;
   sequence=MagickFalse;
   signed_data=(~0UL);
   significant_bits=0;
   use_explicit=MagickFalse;
   explicit_retry = MagickFalse;
   width=0;
   window_center=0;
   window_width=0;
   for (group=0; (group != 0x7FE0) || (element != 0x0010) ||
                 (sequence != MagickFalse); )
   {
      
     image->offset=(ssize_t) TellBlob(image);
     group=ReadBlobLSBShort(image);
     element=ReadBlobLSBShort(image);
     if ((group != 0x0002) && (image->endian == MSBEndian))
       {
         group=(unsigned short) ((group << 8) | ((group >> 8) & 0xFF));
         element=(unsigned short) ((element << 8) | ((element >> 8) & 0xFF));
       }
     quantum=0;
      
     for (i=0; dicom_info[i].group < 0xffff; i++)
       if ((group == dicom_info[i].group) && (element == dicom_info[i].element))
         break;
     (void) CopyMagickString(implicit_vr,dicom_info[i].vr,MaxTextExtent);
     count=ReadBlob(image,2,(unsigned char *) explicit_vr);
     if (count != 2)
       ThrowReaderException(CorruptImageError,"ImproperImageHeader");
      
     if ((explicit_file == MagickFalse) && (group != 0x0002))
       explicit_file=(isupper((unsigned char) *explicit_vr) != MagickFalse) &&
         (isupper((unsigned char) *(explicit_vr+1)) != MagickFalse) ?
         MagickTrue : MagickFalse;
     use_explicit=((group == 0x0002) && (explicit_retry == MagickFalse)) ||
       (explicit_file != MagickFalse) ? MagickTrue : MagickFalse;
     if ((use_explicit != MagickFalse) && (strcmp(implicit_vr,"xs") == 0))
       (void) CopyMagickString(implicit_vr,explicit_vr,MaxTextExtent);
     if ((use_explicit == MagickFalse) || (strcmp(implicit_vr,"!!") == 0))
       {
         offset=SeekBlob(image,(MagickOffsetType) -2,SEEK_CUR);
         if (offset < 0)
           ThrowReaderException(CorruptImageError,"ImproperImageHeader");
         quantum=4;
       }
     else
       {
          
         quantum=2;
         if ((strcmp(explicit_vr,"OB") == 0) ||
             (strcmp(explicit_vr,"UN") == 0) ||
             (strcmp(explicit_vr,"OW") == 0) || (strcmp(explicit_vr,"SQ") == 0))
           {
             (void) ReadBlobLSBShort(image);
             quantum=4;
           }
       }
     datum=0;
     if (quantum == 4)
       {
         if (group == 0x0002)
           datum=(int) ReadBlobLSBLong(image);
         else
           datum=(int) ReadBlobLong(image);
       }
     else
       if (quantum == 2)
         {
           if (group == 0x0002)
             datum=(int) ReadBlobLSBShort(image);
           else
             datum=(int) ReadBlobShort(image);
         }
     quantum=0;
     length=1;
     if (datum != 0)
       {
         if ((strcmp(implicit_vr,"SS") == 0) ||
             (strcmp(implicit_vr,"US") == 0))
           quantum=2;
         else
           if ((strcmp(implicit_vr,"UL") == 0) ||
               (strcmp(implicit_vr,"SL") == 0) ||
               (strcmp(implicit_vr,"FL") == 0))
             quantum=4;
           else
             if (strcmp(implicit_vr,"FD") != 0)
               quantum=1;
             else
               quantum=8;
         if (datum != ~0)
           length=(size_t) datum/quantum;
         else
           {
              
             quantum=0;
             length=0;
           }
       }
     if (image_info->verbose != MagickFalse)
       {
          
         if (use_explicit == MagickFalse)
           explicit_vr[0]='\0';
         for (i=0; dicom_info[i].description != (char *) NULL; i++)
           if ((group == dicom_info[i].group) &&
               (element == dicom_info[i].element))
             break;
         (void) FormatLocaleFile(stdout,"0x%04lX %4ld %s-%s (0x%04lx,0x%04lx)",
           (unsigned long) image->offset,(long) length,implicit_vr,explicit_vr,
           (unsigned long) group,(unsigned long) element);
         if (dicom_info[i].description != (char *) NULL)
           (void) FormatLocaleFile(stdout," %s",dicom_info[i].description);
         (void) FormatLocaleFile(stdout,": ");
       }
     if ((sequence == MagickFalse) && (group == 0x7FE0) && (element == 0x0010))
       {
         if (image_info->verbose != MagickFalse)
           (void) FormatLocaleFile(stdout,"\n");
         break;
       }
      
     data=(unsigned char *) NULL;
     if ((length == 1) && (quantum == 1))
       datum=(int) ReadBlobByte(image);
     else
       if ((length == 1) && (quantum == 2))
         {
           if (group == 0x0002)
             datum=(int) ReadBlobLSBShort(image);
           else
             datum=(int) ReadBlobShort(image);
         }
       else
         if ((length == 1) && (quantum == 4))
           {
             if (group == 0x0002)
               datum=(int) ReadBlobLSBLong(image);
             else
               datum=(int) ReadBlobLong(image);
           }
         else
           if ((quantum != 0) && (length != 0))
             {
               if (~length >= 1)
                 data=(unsigned char *) AcquireQuantumMemory(length+1,quantum*
                   sizeof(*data));
               if (data == (unsigned char *) NULL)
                 ThrowReaderException(ResourceLimitError,
                   "MemoryAllocationFailed");
               count=ReadBlob(image,(size_t) quantum*length,data);
               if (count != (ssize_t) (quantum*length))
                 {
                   if (image_info->verbose != MagickFalse)
                     (void) FormatLocaleFile(stdout,"count=%d quantum=%d "
                       "length=%d group=%d\n",(int) count,(int) quantum,(int)
                       length,(int) group);
                    ThrowReaderException(CorruptImageError,
                      "InsufficientImageDataInFile");
                 }
               data[length*quantum]='\0';
             }
           else
             if ((unsigned int) datum == 0xFFFFFFFFU)
               {
                 sequence=MagickTrue;
                 continue;
               }
     if ((unsigned int) ((group << 16) | element) == 0xFFFEE0DD)
       {
         if (data != (unsigned char *) NULL)
           data=(unsigned char *) RelinquishMagickMemory(data);
         sequence=MagickFalse;
         continue;
       }
     if (sequence != MagickFalse)
       {
         if (data != (unsigned char *) NULL)
           data=(unsigned char *) RelinquishMagickMemory(data);
         continue;
       }
     switch (group)
     {
       case 0x0002:
       {
         switch (element)
         {
           case 0x0010:
           {
             char
               transfer_syntax[MaxTextExtent];
 
              
             if ((datum == 0) && (explicit_retry == MagickFalse))
               {
                 explicit_retry=MagickTrue;
                 (void) SeekBlob(image,(MagickOffsetType) 0,SEEK_SET);
                 group=0;
                 element=0;
                 if (image_info->verbose != MagickFalse)
                   (void) FormatLocaleFile(stdout,
                     "Corrupted image - trying explicit format\n");
                 break;
               }
             *transfer_syntax='\0';
             if (data != (unsigned char *) NULL)
               (void) CopyMagickString(transfer_syntax,(char *) data,
                 MaxTextExtent);
             if (image_info->verbose != MagickFalse)
               (void) FormatLocaleFile(stdout,"transfer_syntax=%s\n",
                 (const char *) transfer_syntax);
             if (strncmp(transfer_syntax,"1.2.840.10008.1.2",17) == 0)
               {
                 int
                   subtype,
                   type;
 
                 type=0;
                 subtype=0;
                 (void) sscanf(transfer_syntax+17,".%d.%d",&type,&subtype);
                 switch (type)
                 {
                   case 1:
                   {
                     image->endian=LSBEndian;
                     break;
                   }
                   case 2:
                   {
                     image->endian=MSBEndian;
                     break;
                   }
                   case 4:
                   {
                     if ((subtype >= 80) && (subtype <= 81))
                       image->compression=JPEGCompression;
                     else
                       if ((subtype >= 90) && (subtype <= 93))
                         image->compression=JPEG2000Compression;
                       else
                         image->compression=JPEGCompression;
                     break;
                   }
                   case 5:
                   {
                     image->compression=RLECompression;
                     break;
                   }
                 }
               }
             break;
           }
           default:
             break;
         }
         break;
       }
       case 0x0028:
       {
         switch (element)
         {
           case 0x0002:
           {
              
             samples_per_pixel=(size_t) datum;
             break;
           }
           case 0x0004:
           {
              
             for (i=0; i < (ssize_t) MagickMin(length,MaxTextExtent-1); i++)
               photometric[i]=(char) data[i];
             photometric[i]='\0';
             polarity=LocaleCompare(photometric,"MONOCHROME1 ") == 0 ?
               MagickTrue : MagickFalse;
             break;
           }
           case 0x0006:
           {
              
             if (datum == 1)
               image->interlace=PlaneInterlace;
             break;
           }
           case 0x0008:
           {
              
             number_scenes=StringToUnsignedLong((char *) data);
             break;
           }
           case 0x0010:
           {
              
             height=(size_t) datum;
             break;
           }
           case 0x0011:
           {
              
             width=(size_t) datum;
             break;
           }
           case 0x0100:
           {
              
             bits_allocated=(size_t) datum;
             bytes_per_pixel=1;
             if (datum > 8)
               bytes_per_pixel=2;
             depth=bits_allocated;
             if (depth > 32)
               ThrowReaderException(CorruptImageError,"ImproperImageHeader");
             max_value=(1UL << bits_allocated)-1;
             break;
           }
           case 0x0101:
           {
              
             significant_bits=(size_t) datum;
             bytes_per_pixel=1;
             if (significant_bits > 8)
               bytes_per_pixel=2;
             depth=significant_bits;
             if (depth > 32)
               ThrowReaderException(CorruptImageError,"ImproperImageHeader");
             max_value=(1UL << significant_bits)-1;
             mask=(size_t) GetQuantumRange(significant_bits);
             break;
           }
           case 0x0102:
           {
              
             break;
           }
           case 0x0103:
           {
              
             signed_data=(size_t) datum;
             break;
           }
           case 0x1050:
           {
              
             if (data != (unsigned char *) NULL)
               window_center=(ssize_t) StringToLong((char *) data);
             break;
           }
           case 0x1051:
           {
              
             if (data != (unsigned char *) NULL)
               window_width=StringToUnsignedLong((char *) data);
             break;
           }
           case 0x1200:
           case 0x3006:
           {
              
             if (data == (unsigned char *) NULL)
               break;
             colors=(size_t) (length/bytes_per_pixel);
             datum=(int) colors;
             graymap=(int *) AcquireQuantumMemory((size_t) colors,
               sizeof(*graymap));
             if (graymap == (int *) NULL)
               ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
             for (i=0; i < (ssize_t) colors; i++)
               if (bytes_per_pixel == 1)
                 graymap[i]=(int) data[i];
               else
                 graymap[i]=(int) ((short *) data)[i];
             break;
           }
           case 0x1201:
           {
             unsigned short
               index;
 
              
             if (data == (unsigned char *) NULL)
               break;
             colors=(size_t) (length/2);
             datum=(int) colors;
             redmap=(int *) AcquireQuantumMemory((size_t) colors,
               sizeof(*redmap));
             if (redmap == (int *) NULL)
               ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
             p=data;
             for (i=0; i < (ssize_t) colors; i++)
             {
               if (image->endian == MSBEndian)
                 index=(unsigned short) ((*p << 8) | *(p+1));
               else
                 index=(unsigned short) (*p | (*(p+1) << 8));
               redmap[i]=(int) index;
               p+=2;
             }
             break;
           }
           case 0x1202:
           {
             unsigned short
               index;
 
              
             if (data == (unsigned char *) NULL)
               break;
             colors=(size_t) (length/2);
             datum=(int) colors;
             greenmap=(int *) AcquireQuantumMemory((size_t) colors,
               sizeof(*greenmap));
             if (greenmap == (int *) NULL)
               ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
             p=data;
             for (i=0; i < (ssize_t) colors; i++)
             {
               if (image->endian == MSBEndian)
                 index=(unsigned short) ((*p << 8) | *(p+1));
               else
                 index=(unsigned short) (*p | (*(p+1) << 8));
               greenmap[i]=(int) index;
               p+=2;
             }
             break;
           }
           case 0x1203:
           {
             unsigned short
               index;
 
              
             if (data == (unsigned char *) NULL)
               break;
             colors=(size_t) (length/2);
             datum=(int) colors;
             bluemap=(int *) AcquireQuantumMemory((size_t) colors,
               sizeof(*bluemap));
             if (bluemap == (int *) NULL)
               ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
             p=data;
             for (i=0; i < (ssize_t) colors; i++)
             {
               if (image->endian == MSBEndian)
                 index=(unsigned short) ((*p << 8) | *(p+1));
               else
                 index=(unsigned short) (*p | (*(p+1) << 8));
               bluemap[i]=(int) index;
               p+=2;
             }
             break;
           }
           default:
             break;
         }
         break;
       }
       case 0x2050:
       {
         switch (element)
         {
           case 0x0020:
           {
             if ((data != (unsigned char *) NULL) &&
                 (strncmp((char*) data,"INVERSE", 7) == 0))
               polarity=MagickTrue;
             break;
           }
           default:
             break;
         }
         break;
       }
       default:
         break;
     }
     if (data != (unsigned char *) NULL)
       {
         char
           *attribute;
 
         for (i=0; dicom_info[i].description != (char *) NULL; i++)
           if ((group == dicom_info[i].group) &&
               (element == dicom_info[i].element))
             break;
         if (dicom_info[i].description != (char *) NULL)
           {
             attribute=AcquireString("dcm:");
             (void) ConcatenateString(&attribute,dicom_info[i].description);
             for (i=0; i < (ssize_t) MagickMax(length,4); i++)
               if (isprint((int) data[i]) == MagickFalse)
                 break;
             if ((i == (ssize_t) length) || (length > 4))
               {
                 (void) SubstituteString(&attribute," ","");
                 (void) SetImageProperty(image,attribute,(char *) data);
               }
             attribute=DestroyString(attribute);
           }
       }
     if (image_info->verbose != MagickFalse)
       {
         if (data == (unsigned char *) NULL)
           (void) FormatLocaleFile(stdout,"%d\n",datum);
         else
           {
              
             for (i=0; i < (ssize_t) MagickMax(length,4); i++)
               if (isprint((int) data[i]) == MagickFalse)
                 break;
             if ((i != (ssize_t) length) && (length <= 4))
               {
                 ssize_t
                   j;
 
                 datum=0;
                 for (j=(ssize_t) length-1; j >= 0; j--)
                   datum=(256*datum+data[j]);
                 (void) FormatLocaleFile(stdout,"%d",datum);
               }
             else
               for (i=0; i < (ssize_t) length; i++)
                 if (isprint((int) data[i]) != MagickFalse)
                   (void) FormatLocaleFile(stdout,"%c",data[i]);
                 else
                   (void) FormatLocaleFile(stdout,"%c",'.');
             (void) FormatLocaleFile(stdout,"\n");
           }
       }
     if (data != (unsigned char *) NULL)
       data=(unsigned char *) RelinquishMagickMemory(data);
     if (EOFBlob(image) != MagickFalse)
       {
         ThrowFileException(exception,CorruptImageError,"UnexpectedEndOfFile",
           image->filename);
         break;
       }
   }
   if ((width == 0) || (height == 0))
     ThrowReaderException(CorruptImageError,"ImproperImageHeader");
   image->columns=(size_t) width;
   image->rows=(size_t) height;
   if (signed_data == 0xffff)
     signed_data=(size_t) (significant_bits == 16 ? 1 : 0);
   if ((image->compression == JPEGCompression) ||
       (image->compression == JPEG2000Compression))
     {
       Image
         *images;
 
       ImageInfo
         *read_info;
 
       int
         c;
 
       size_t
         length;
 
       unsigned int
         tag;
 
        
       for (i=0; i < (ssize_t) stream_info->remaining; i++)
         (void) ReadBlobByte(image);
       tag=(ReadBlobLSBShort(image) << 16) | ReadBlobLSBShort(image);
       (void) tag;
       length=(size_t) ReadBlobLSBLong(image);
       stream_info->offset_count=length >> 2;
       if (stream_info->offset_count != 0)
         {
           MagickOffsetType
             offset;
 
           stream_info->offsets=(ssize_t *) AcquireQuantumMemory(
             stream_info->offset_count,sizeof(*stream_info->offsets));
           if (stream_info->offsets == (ssize_t *) NULL)
             ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
           for (i=0; i < (ssize_t) stream_info->offset_count; i++)
             stream_info->offsets[i]=(ssize_t) ((int) ReadBlobLSBLong(image));
           offset=TellBlob(image);
           for (i=0; i < (ssize_t) stream_info->offset_count; i++)
             stream_info->offsets[i]+=offset;
         }
        
       read_info=CloneImageInfo(image_info);
       SetImageInfoBlob(read_info,(void *) NULL,0);
       images=NewImageList();
       for (scene=0; scene < (ssize_t) number_scenes; scene++)
       {
         char
           filename[MaxTextExtent];
 
         const char
           *property;
 
         FILE
           *file;
 
         Image
           *jpeg_image;
 
         int
           unique_file;
 
         unsigned int
           tag;
 
         tag=(ReadBlobLSBShort(image) << 16) | ReadBlobLSBShort(image);
         length=(size_t) ReadBlobLSBLong(image);
         if (tag == 0xFFFEE0DD)
           break;  
         if (tag != 0xFFFEE000)
           ThrowReaderException(CorruptImageError,"ImproperImageHeader");
         file=(FILE *) NULL;
         unique_file=AcquireUniqueFileResource(filename);
         if (unique_file != -1)
           file=fdopen(unique_file,"wb");
         if (file == (FILE *) NULL)
           {
             (void) RelinquishUniqueFileResource(filename);
             ThrowFileException(exception,FileOpenError,
               "UnableToCreateTemporaryFile",filename);
             break;
           }
         for ( ; length != 0; length--)
         {
           c=ReadBlobByte(image);
           if (c == EOF)
             {
               ThrowFileException(exception,CorruptImageError,
                 "UnexpectedEndOfFile",image->filename);
               break;
             }
           (void) fputc(c,file);
         }
         (void) fclose(file);
         (void) FormatLocaleString(read_info->filename,MaxTextExtent,"jpeg:%s",
           filename);
         if (image->compression == JPEG2000Compression)
           (void) FormatLocaleString(read_info->filename,MaxTextExtent,"j2k:%s",
             filename);
         jpeg_image=ReadImage(read_info,exception);
         if (jpeg_image != (Image *) NULL)
           {
             ResetImagePropertyIterator(image);
             property=GetNextImageProperty(image);
             while (property != (const char *) NULL)
             {
               (void) SetImageProperty(jpeg_image,property,
                 GetImageProperty(image,property));
               property=GetNextImageProperty(image);
             }
             AppendImageToList(&images,jpeg_image);
           }
         (void) RelinquishUniqueFileResource(filename);
       }
       read_info=DestroyImageInfo(read_info);
       image=DestroyImage(image);
       return(GetFirstImageInList(images));
     }
   if (depth != (1UL*MAGICKCORE_QUANTUM_DEPTH))
     {
       QuantumAny
         range;
 
       size_t
         length;
 
        
       length=(size_t) (GetQuantumRange(depth)+1);
       scale=(Quantum *) AcquireQuantumMemory(length,sizeof(*scale));
       if (scale == (Quantum *) NULL)
         ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
       range=GetQuantumRange(depth);
       for (i=0; i < (ssize_t) (GetQuantumRange(depth)+1); i++)
         scale[i]=ScaleAnyToQuantum((size_t) i,range);
     }
   if (image->compression == RLECompression)
     {
       size_t
         length;
 
       unsigned int
         tag;
 
        
       for (i=0; i < (ssize_t) stream_info->remaining; i++)
         (void) ReadBlobByte(image);
       tag=(ReadBlobLSBShort(image) << 16) | ReadBlobLSBShort(image);
       (void) tag;
       length=(size_t) ReadBlobLSBLong(image);
       stream_info->offset_count=length >> 2;
       if (stream_info->offset_count != 0)
         {
           MagickOffsetType
             offset;
 
           stream_info->offsets=(ssize_t *) AcquireQuantumMemory(
             stream_info->offset_count,sizeof(*stream_info->offsets));
           if (stream_info->offsets == (ssize_t *) NULL)
             ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
           for (i=0; i < (ssize_t) stream_info->offset_count; i++)
             stream_info->offsets[i]=(ssize_t) ((int) ReadBlobLSBLong(image));
           offset=TellBlob(image);
           for (i=0; i < (ssize_t) stream_info->offset_count; i++)
             stream_info->offsets[i]+=offset;
         }
     }
   for (scene=0; scene < (ssize_t) number_scenes; scene++)
   {
     if (image_info->ping != MagickFalse)
       break;
      image->columns=(size_t) width;
      image->rows=(size_t) height;
      image->depth=depth;
//     status=SetImageExtent(image,image->columns,image->rows);
//     if (status == MagickFalse)
//       {
//         InheritException(exception,&image->exception);
//         break;
//       }
      image->colorspace=RGBColorspace;
      if ((image->colormap == (PixelPacket *) NULL) && (samples_per_pixel == 1))
        {
         size_t
           one;
 
         one=1;
         if (colors == 0)
           colors=one << depth;
         if (AcquireImageColormap(image,one << depth) == MagickFalse)
           ThrowReaderException(ResourceLimitError,"MemoryAllocationFailed");
         if (redmap != (int *) NULL)
           for (i=0; i < (ssize_t) colors; i++)
           {
             index=redmap[i];
             if ((scale != (Quantum *) NULL) && (index <= (int) max_value))
               index=(int) scale[index];
             image->colormap[i].red=(Quantum) index;
           }
         if (greenmap != (int *) NULL)
           for (i=0; i < (ssize_t) colors; i++)
           {
             index=greenmap[i];
             if ((scale != (Quantum *) NULL) && (index <= (int) max_value))
               index=(int) scale[index];
             image->colormap[i].green=(Quantum) index;
           }
         if (bluemap != (int *) NULL)
           for (i=0; i < (ssize_t) colors; i++)
           {
             index=bluemap[i];
             if ((scale != (Quantum *) NULL) && (index <= (int) max_value))
               index=(int) scale[index];
             image->colormap[i].blue=(Quantum) index;
           }
         if (graymap != (int *) NULL)
           for (i=0; i < (ssize_t) colors; i++)
           {
             index=graymap[i];
             if ((scale != (Quantum *) NULL) && (index <= (int) max_value))
               index=(int) scale[index];
             image->colormap[i].red=(Quantum) index;
             image->colormap[i].green=(Quantum) index;
             image->colormap[i].blue=(Quantum) index;
           }
       }
     if (image->compression == RLECompression)
       {
         unsigned int
           tag;
 
          
         for (i=0; i < (ssize_t) stream_info->remaining; i++)
           (void) ReadBlobByte(image);
         tag=(ReadBlobLSBShort(image) << 16) | ReadBlobLSBShort(image);
         stream_info->remaining=(size_t) ReadBlobLSBLong(image);
         if ((tag != 0xFFFEE000) || (stream_info->remaining <= 64) ||
             (EOFBlob(image) != MagickFalse))
           ThrowReaderException(CorruptImageError,"ImproperImageHeader");
         stream_info->count=0;
         stream_info->segment_count=ReadBlobLSBLong(image);
         if (stream_info->segment_count > 1)
           {
             bytes_per_pixel=1;
             depth=8;
           }
         for (i=0; i < 15; i++)
           stream_info->segments[i]=(ssize_t) ((int) ReadBlobLSBLong(image));
         stream_info->remaining-=64;
       }
     if ((samples_per_pixel > 1) && (image->interlace == PlaneInterlace))
       {
          
         for (i=0; i < (ssize_t) samples_per_pixel; i++)
         {
           for (y=0; y < (ssize_t) image->rows; y++)
           {
             q=GetAuthenticPixels(image,0,y,image->columns,1,exception);
             if (q == (PixelPacket *) NULL)
               break;
             for (x=0; x < (ssize_t) image->columns; x++)
             {
               switch ((int) i)
               {
                 case 0:
                 {
                   SetPixelRed(q,ScaleCharToQuantum((unsigned char)
                     ReadDCMByte(stream_info,image)));
                   break;
                 }
                 case 1:
                 {
                   SetPixelGreen(q,ScaleCharToQuantum((unsigned char)
                     ReadDCMByte(stream_info,image)));
                   break;
                 }
                 case 2:
                 {
                   SetPixelBlue(q,ScaleCharToQuantum((unsigned char)
                     ReadDCMByte(stream_info,image)));
                   break;
                 }
                 case 3:
                 {
                   SetPixelAlpha(q,ScaleCharToQuantum((unsigned char)
                     ReadDCMByte(stream_info,image)));
                   break;
                 }
                 default:
                   break;
               }
               q++;
             }
             if (SyncAuthenticPixels(image,exception) == MagickFalse)
               break;
             if (image->previous == (Image *) NULL)
               {
                 status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
                   image->rows);
                 if (status == MagickFalse)
                   break;
               }
           }
         }
       }
     else
       {
         const char
           *option;
 
         int
           byte;
 
         LongPixelPacket
           pixel;
 
          
         byte=0;
         i=0;
         if ((window_center != 0) && (window_width == 0))
           window_width=(size_t) window_center;
         option=GetImageOption(image_info,"dcm:display-range");
         if (option != (const char *) NULL)
           {
             if (LocaleCompare(option,"reset") == 0)
               window_width=0;
           }
         (void) ResetMagickMemory(&pixel,0,sizeof(pixel));
         for (y=0; y < (ssize_t) image->rows; y++)
         {
           q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
           if (q == (PixelPacket *) NULL)
             break;
           indexes=GetAuthenticIndexQueue(image);
           for (x=0; x < (ssize_t) image->columns; x++)
           {
             if (samples_per_pixel == 1)
               {
                 int
                   pixel_value;
 
                 if (bytes_per_pixel == 1)
                   pixel_value=polarity != MagickFalse ? ((int) max_value-
                     ReadDCMByte(stream_info,image)) :
                     ReadDCMByte(stream_info,image);
                 else
                   if ((bits_allocated != 12) || (significant_bits != 12))
                     pixel_value=(int) (polarity != MagickFalse ? (max_value-
                       ReadDCMShort(stream_info,image)) :
                       ReadDCMShort(stream_info,image));
                   else
                     {
                       if ((i & 0x01) != 0)
                         pixel_value=(ReadDCMByte(stream_info,image) << 8) |
                           byte;
                       else
                         {
                           pixel_value=(int) ReadDCMShort(stream_info,image);
                           byte=(int) (pixel_value & 0x0f);
                           pixel_value>>=4;
                         }
                       i++;
                     }
                 index=pixel_value;
                 if (window_width == 0)
                   {
                     if (signed_data == 1)
                       index=pixel_value-32767;
                   }
                 else
                   {
                     ssize_t
                       window_max,
                       window_min;
 
                     window_min=(ssize_t) ceil((double) window_center-
                       (window_width-1.0)/2.0-0.5);
                     window_max=(ssize_t) floor((double) window_center+
                       (window_width-1.0)/2.0+0.5);
                     if ((ssize_t) pixel_value <= window_min)
                       index=0;
                     else
                       if ((ssize_t) pixel_value > window_max)
                         index=(int) max_value;
                       else
                         index=(int) (max_value*(((pixel_value-window_center-
                           0.5)/(window_width-1))+0.5));
                   }
                 index&=mask;
                 index=(int) ConstrainColormapIndex(image,(size_t) index);
                 SetPixelIndex(indexes+x,index);
                 pixel.red=1U*image->colormap[index].red;
                 pixel.green=1U*image->colormap[index].green;
                 pixel.blue=1U*image->colormap[index].blue;
               }
             else
               {
                 if (bytes_per_pixel == 1)
                   {
                     pixel.red=(unsigned int) ReadDCMByte(stream_info,image);
                     pixel.green=(unsigned int) ReadDCMByte(stream_info,image);
                     pixel.blue=(unsigned int) ReadDCMByte(stream_info,image);
                   }
                 else
                   {
                     pixel.red=ReadDCMShort(stream_info,image);
                     pixel.green=ReadDCMShort(stream_info,image);
                     pixel.blue=ReadDCMShort(stream_info,image);
                   }
                 pixel.red&=mask;
                 pixel.green&=mask;
                 pixel.blue&=mask;
                 if (scale != (Quantum *) NULL)
                   {
                     pixel.red=scale[pixel.red];
                     pixel.green=scale[pixel.green];
                     pixel.blue=scale[pixel.blue];
                   }
               }
             SetPixelRed(q,pixel.red);
             SetPixelGreen(q,pixel.green);
             SetPixelBlue(q,pixel.blue);
             q++;
           }
           if (SyncAuthenticPixels(image,exception) == MagickFalse)
             break;
           if (image->previous == (Image *) NULL)
             {
               status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
                 image->rows);
               if (status == MagickFalse)
                 break;
             }
         }
         if (stream_info->segment_count > 1)
           for (y=0; y < (ssize_t) image->rows; y++)
           {
             q=QueueAuthenticPixels(image,0,y,image->columns,1,exception);
             if (q == (PixelPacket *) NULL)
               break;
             indexes=GetAuthenticIndexQueue(image);
             for (x=0; x < (ssize_t) image->columns; x++)
             {
               if (samples_per_pixel == 1)
                 {
                   int
                     pixel_value;
 
                   if (bytes_per_pixel == 1)
                     pixel_value=polarity != MagickFalse ? ((int) max_value-
                       ReadDCMByte(stream_info,image)) :
                       ReadDCMByte(stream_info,image);
                   else
                     if ((bits_allocated != 12) || (significant_bits != 12))
                       {
                         pixel_value=(int) (polarity != MagickFalse ? (max_value-
                           ReadDCMShort(stream_info,image)) :
                           ReadDCMShort(stream_info,image));
                         if (signed_data == 1)
                           pixel_value=((signed short) pixel_value);
                       }
                     else
                       {
                         if ((i & 0x01) != 0)
                           pixel_value=(ReadDCMByte(stream_info,image) << 8) |
                             byte;
                         else
                           {
                             pixel_value=(int) ReadDCMShort(stream_info,image);
                             byte=(int) (pixel_value & 0x0f);
                             pixel_value>>=4;
                           }
                         i++;
                       }
                   index=pixel_value;
                   if (window_width == 0)
                     {
                       if (signed_data == 1)
                         index=pixel_value-32767;
                     }
                   else
                     {
                       ssize_t
                         window_max,
                         window_min;
 
                       window_min=(ssize_t) ceil((double) window_center-
                         (window_width-1.0)/2.0-0.5);
                       window_max=(ssize_t) floor((double) window_center+
                         (window_width-1.0)/2.0+0.5);
                       if ((ssize_t) pixel_value <= window_min)
                         index=0;
                       else
                         if ((ssize_t) pixel_value > window_max)
                           index=(int) max_value;
                         else
                           index=(int) (max_value*(((pixel_value-window_center-
                             0.5)/(window_width-1))+0.5));
                     }
                   index&=mask;
                   index=(int) ConstrainColormapIndex(image,(size_t) index);
                   SetPixelIndex(indexes+x,(((size_t) GetPixelIndex(indexes+x)) |
                     (((size_t) index) << 8)));
                   pixel.red=1U*image->colormap[index].red;
                   pixel.green=1U*image->colormap[index].green;
                   pixel.blue=1U*image->colormap[index].blue;
                 }
               else
                 {
                   if (bytes_per_pixel == 1)
                     {
                       pixel.red=(unsigned int) ReadDCMByte(stream_info,image);
                       pixel.green=(unsigned int) ReadDCMByte(stream_info,image);
                       pixel.blue=(unsigned int) ReadDCMByte(stream_info,image);
                     }
                   else
                     {
                       pixel.red=ReadDCMShort(stream_info,image);
                       pixel.green=ReadDCMShort(stream_info,image);
                       pixel.blue=ReadDCMShort(stream_info,image);
                     }
                   pixel.red&=mask;
                   pixel.green&=mask;
                   pixel.blue&=mask;
                   if (scale != (Quantum *) NULL)
                     {
                       pixel.red=scale[pixel.red];
                       pixel.green=scale[pixel.green];
                       pixel.blue=scale[pixel.blue];
                     }
                 }
               SetPixelRed(q,(((size_t) GetPixelRed(q)) |
                 (((size_t) pixel.red) << 8)));
               SetPixelGreen(q,(((size_t) GetPixelGreen(q)) |
                 (((size_t) pixel.green) << 8)));
               SetPixelBlue(q,(((size_t) GetPixelBlue(q)) |
                 (((size_t) pixel.blue) << 8)));
               q++;
             }
             if (SyncAuthenticPixels(image,exception) == MagickFalse)
               break;
             if (image->previous == (Image *) NULL)
               {
                 status=SetImageProgress(image,LoadImageTag,(MagickOffsetType) y,
                   image->rows);
                 if (status == MagickFalse)
                   break;
               }
           }
       }
     if (IsGrayImage(image,exception) != MagickFalse)
       (void) SetImageColorspace(image,GRAYColorspace);
     if (EOFBlob(image) != MagickFalse)
       {
         ThrowFileException(exception,CorruptImageError,"UnexpectedEndOfFile",
           image->filename);
         break;
       }
      
     if (image_info->number_scenes != 0)
       if (image->scene >= (image_info->scene+image_info->number_scenes-1))
         break;
     if (scene < (ssize_t) (number_scenes-1))
       {
          
         AcquireNextImage(image_info,image);
         if (GetNextImageInList(image) == (Image *) NULL)
           {
             image=DestroyImageList(image);
             return((Image *) NULL);
           }
         image=SyncNextImageInList(image);
         status=SetImageProgress(image,LoadImagesTag,TellBlob(image),
           GetBlobSize(image));
         if (status == MagickFalse)
           break;
       }
   }
    
   if (stream_info->offsets != (ssize_t *) NULL)
     stream_info->offsets=(ssize_t *)
       RelinquishMagickMemory(stream_info->offsets);
   stream_info=(DCMStreamInfo *) RelinquishMagickMemory(stream_info);
   if (scale != (Quantum *) NULL)
     scale=(Quantum *) RelinquishMagickMemory(scale);
   if (graymap != (int *) NULL)
     graymap=(int *) RelinquishMagickMemory(graymap);
   if (bluemap != (int *) NULL)
     bluemap=(int *) RelinquishMagickMemory(bluemap);
   if (greenmap != (int *) NULL)
     greenmap=(int *) RelinquishMagickMemory(greenmap);
   if (redmap != (int *) NULL)
     redmap=(int *) RelinquishMagickMemory(redmap);
   (void) CloseBlob(image);
   return(GetFirstImageInList(image));
 }
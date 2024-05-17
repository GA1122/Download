static MagickBooleanType WriteVIFFImage(const ImageInfo *image_info,
  Image *image)
{
#define VFF_CM_genericRGB  15
#define VFF_CM_NONE  0
#define VFF_DEP_IEEEORDER  0x2
#define VFF_DES_RAW  0
#define VFF_LOC_IMPLICIT  1
#define VFF_MAPTYP_NONE  0
#define VFF_MAPTYP_1_BYTE  1
#define VFF_MS_NONE  0
#define VFF_MS_ONEPERBAND  1
#define VFF_TYP_BIT  0
#define VFF_TYP_1_BYTE  1

  typedef struct _ViffInfo
  {
    char
      identifier,
      file_type,
      release,
      version,
      machine_dependency,
      reserve[3],
      comment[512];

    size_t
      rows,
      columns,
      subrows;

    int
      x_offset,
      y_offset;

    unsigned int
      x_bits_per_pixel,
      y_bits_per_pixel,
      location_type,
      location_dimension,
      number_of_images,
      number_data_bands,
      data_storage_type,
      data_encode_scheme,
      map_scheme,
      map_storage_type,
      map_rows,
      map_columns,
      map_subrows,
      map_enable,
      maps_per_cycle,
      color_space_model;
  } ViffInfo;

  const char
    *value;

  MagickBooleanType
    status;

  MagickOffsetType
    scene;

  MagickSizeType
    number_pixels,
    packets;

  MemoryInfo
    *pixel_info;

  register const IndexPacket
    *indexes;

  register const PixelPacket
    *p;

  register ssize_t
    x;

  register ssize_t
    i;

  register unsigned char
    *q;

  ssize_t
    y;

  unsigned char
    *pixels;

  ViffInfo
    viff_info;

   
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  status=OpenBlob(image_info,image,WriteBinaryBlobMode,&image->exception);
  if (status == MagickFalse)
    return(status);
  (void) ResetMagickMemory(&viff_info,0,sizeof(ViffInfo));
  scene=0;
  do
  {
     
    (void) TransformImageColorspace(image,sRGBColorspace);
DisableMSCWarning(4310)
    viff_info.identifier=(char) 0xab;
RestoreMSCWarning
    viff_info.file_type=1;
    viff_info.release=1;
    viff_info.version=3;
    viff_info.machine_dependency=VFF_DEP_IEEEORDER;   
    *viff_info.comment='\0';
    value=GetImageProperty(image,"comment");
    if (value != (const char *) NULL)
      (void) CopyMagickString(viff_info.comment,value,MagickMin(strlen(value),
        511)+1);
    viff_info.rows=image->columns;
    viff_info.columns=image->rows;
    viff_info.subrows=0;
    viff_info.x_offset=(~0);
    viff_info.y_offset=(~0);
    viff_info.x_bits_per_pixel=0;
    viff_info.y_bits_per_pixel=0;
    viff_info.location_type=VFF_LOC_IMPLICIT;
    viff_info.location_dimension=0;
    viff_info.number_of_images=1;
    viff_info.data_encode_scheme=VFF_DES_RAW;
    viff_info.map_scheme=VFF_MS_NONE;
    viff_info.map_storage_type=VFF_MAPTYP_NONE;
    viff_info.map_rows=0;
    viff_info.map_columns=0;
    viff_info.map_subrows=0;
    viff_info.map_enable=1;   
    viff_info.maps_per_cycle=0;
    number_pixels=(MagickSizeType) image->columns*image->rows;
    if (image->storage_class == DirectClass)
      {
         
        viff_info.number_data_bands=image->matte ? 4U : 3U;
        viff_info.color_space_model=VFF_CM_genericRGB;
        viff_info.data_storage_type=VFF_TYP_1_BYTE;
        packets=viff_info.number_data_bands*number_pixels;
      }
    else
      {
        viff_info.number_data_bands=1;
        viff_info.color_space_model=VFF_CM_NONE;
        viff_info.data_storage_type=VFF_TYP_1_BYTE;
        packets=number_pixels;
        if (SetImageGray(image,&image->exception) == MagickFalse)
          {
             
            viff_info.map_scheme=VFF_MS_ONEPERBAND;
            viff_info.map_storage_type=VFF_MAPTYP_1_BYTE;
            viff_info.map_rows=3;
            viff_info.map_columns=(unsigned int) image->colors;
          }
        else
          if (image->colors <= 2)
            {
               
              viff_info.data_storage_type=VFF_TYP_BIT;
              packets=((image->columns+7) >> 3)*image->rows;
            }
      }
     
    (void) WriteBlob(image,sizeof(viff_info.identifier),(unsigned char *)
      &viff_info.identifier);
    (void) WriteBlob(image,sizeof(viff_info.file_type),(unsigned char *)
      &viff_info.file_type);
    (void) WriteBlob(image,sizeof(viff_info.release),(unsigned char *)
      &viff_info.release);
    (void) WriteBlob(image,sizeof(viff_info.version),(unsigned char *)
      &viff_info.version);
    (void) WriteBlob(image,sizeof(viff_info.machine_dependency),
      (unsigned char *) &viff_info.machine_dependency);
    (void) WriteBlob(image,sizeof(viff_info.reserve),(unsigned char *)
      viff_info.reserve);
    (void) WriteBlob(image,512,(unsigned char *) viff_info.comment);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.rows);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.columns);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.subrows);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.x_offset);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.y_offset);
    viff_info.x_bits_per_pixel=(unsigned int) ((63 << 24) | (128 << 16));
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.x_bits_per_pixel);
    viff_info.y_bits_per_pixel=(unsigned int) ((63 << 24) | (128 << 16));
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.y_bits_per_pixel);
    (void) WriteBlobMSBLong(image,viff_info.location_type);
    (void) WriteBlobMSBLong(image,viff_info.location_dimension);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.number_of_images);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.number_data_bands);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.data_storage_type);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.data_encode_scheme);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_scheme);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_storage_type);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_rows);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_columns);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_subrows);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.map_enable);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.maps_per_cycle);
    (void) WriteBlobMSBLong(image,(unsigned int) viff_info.color_space_model);
    for (i=0; i < 420; i++)
      (void) WriteBlobByte(image,'\0');
     
    pixel_info=AcquireVirtualMemory((size_t) packets,sizeof(*pixels));
    if (pixel_info == (MemoryInfo *) NULL)
      ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
    pixels=(unsigned char *) GetVirtualMemoryBlob(pixel_info);
    q=pixels;
    if (image->storage_class == DirectClass)
      {
         
        number_pixels=(MagickSizeType) image->columns*image->rows;
        for (y=0; y < (ssize_t) image->rows; y++)
        {
          p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
          if (p == (const PixelPacket *) NULL)
            break;
          for (x=0; x < (ssize_t) image->columns; x++)
          {
            *q=ScaleQuantumToChar(GetPixelRed(p));
            *(q+number_pixels)=ScaleQuantumToChar(GetPixelGreen(p));
            *(q+number_pixels*2)=ScaleQuantumToChar(GetPixelBlue(p));
            if (image->matte != MagickFalse)
              *(q+number_pixels*3)=ScaleQuantumToChar((Quantum)
                (GetPixelAlpha(p)));
            p++;
            q++;
          }
          if (image->previous == (Image *) NULL)
            {
              status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
              if (status == MagickFalse)
                break;
            }
        }
      }
    else
      if (SetImageGray(image,&image->exception) == MagickFalse)
        {
          unsigned char
            *viff_colormap;

           
          viff_colormap=(unsigned char *) AcquireQuantumMemory(image->colors,
            3*sizeof(*viff_colormap));
          if (viff_colormap == (unsigned char *) NULL)
            ThrowWriterException(ResourceLimitError,"MemoryAllocationFailed");
          q=viff_colormap;
          for (i=0; i < (ssize_t) image->colors; i++)
            *q++=ScaleQuantumToChar(image->colormap[i].red);
          for (i=0; i < (ssize_t) image->colors; i++)
            *q++=ScaleQuantumToChar(image->colormap[i].green);
          for (i=0; i < (ssize_t) image->colors; i++)
            *q++=ScaleQuantumToChar(image->colormap[i].blue);
          (void) WriteBlob(image,3*image->colors,viff_colormap);
          viff_colormap=(unsigned char *) RelinquishMagickMemory(viff_colormap);
           
          q=pixels;
          for (y=0; y < (ssize_t) image->rows; y++)
          {
            p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
            if (p == (const PixelPacket *) NULL)
              break;
            indexes=GetVirtualIndexQueue(image);
            for (x=0; x < (ssize_t) image->columns; x++)
              *q++=(unsigned char) GetPixelIndex(indexes+x);
            if (image->previous == (Image *) NULL)
              {
                status=SetImageProgress(image,SaveImageTag,(MagickOffsetType) y,
                image->rows);
                if (status == MagickFalse)
                  break;
              }
          }
        }
      else
        if (image->colors <= 2)
          {
            ssize_t
              x,
              y;

            register unsigned char
              bit,
              byte;

             
            (void) SetImageType(image,BilevelType);
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              indexes=GetVirtualIndexQueue(image);
              bit=0;
              byte=0;
              for (x=0; x < (ssize_t) image->columns; x++)
              {
                byte>>=1;
                if (GetPixelLuma(image,p) < (QuantumRange/2.0))
                  byte|=0x80;
                bit++;
                if (bit == 8)
                  {
                    *q++=byte;
                    bit=0;
                    byte=0;
                  }
              }
              if (bit != 0)
                *q++=byte >> (8-bit);
              if (image->previous == (Image *) NULL)
                {
                  status=SetImageProgress(image,SaveImageTag,(MagickOffsetType)
                    y,image->rows);
                  if (status == MagickFalse)
                    break;
                }
            }
          }
        else
          {
             
            for (y=0; y < (ssize_t) image->rows; y++)
            {
              p=GetVirtualPixels(image,0,y,image->columns,1,&image->exception);
              if (p == (const PixelPacket *) NULL)
                break;
              for (x=0; x < (ssize_t) image->columns; x++)
              {
                *q++=(unsigned char) ClampToQuantum(GetPixelLuma(image,p));
                p++;
              }
              if (image->previous == (Image *) NULL)
                {
                  status=SetImageProgress(image,SaveImageTag,(MagickOffsetType)
                    y,image->rows);
                  if (status == MagickFalse)
                    break;
                }
            }
          }
    (void) WriteBlob(image,(size_t) packets,pixels);
    pixel_info=RelinquishVirtualMemory(pixel_info);
    if (GetNextImageInList(image) == (Image *) NULL)
      break;
    image=SyncNextImageInList(image);
    status=SetImageProgress(image,SaveImagesTag,scene++,
      GetImageListLength(image));
    if (status == MagickFalse)
      break;
  } while (image_info->adjoin != MagickFalse);
  (void) CloseBlob(image);
  return(MagickTrue);
}
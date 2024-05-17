 start_input_bmp(j_compress_ptr cinfo, cjpeg_source_ptr sinfo)
 {
   bmp_source_ptr source = (bmp_source_ptr)sinfo;
   U_CHAR bmpfileheader[14];
   U_CHAR bmpinfoheader[64];
 
 #define GET_2B(array, offset) \
   ((unsigned short)UCH(array[offset]) + \
    (((unsigned short)UCH(array[offset + 1])) << 8))
 #define GET_4B(array, offset) \
   ((unsigned int)UCH(array[offset]) + \
    (((unsigned int)UCH(array[offset + 1])) << 8) + \
    (((unsigned int)UCH(array[offset + 2])) << 16) + \
    (((unsigned int)UCH(array[offset + 3])) << 24))
 
   unsigned int bfOffBits;
   unsigned int headerSize;
   int biWidth;
   int biHeight;
   unsigned short biPlanes;
   unsigned int biCompression;
   int biXPelsPerMeter, biYPelsPerMeter;
   unsigned int biClrUsed = 0;
   int mapentrysize = 0;          
   int bPad;
   JDIMENSION row_width = 0;
 
    
   if (!ReadOK(source->pub.input_file, bmpfileheader, 14))
     ERREXIT(cinfo, JERR_INPUT_EOF);
   if (GET_2B(bmpfileheader, 0) != 0x4D42)  
     ERREXIT(cinfo, JERR_BMP_NOT);
   bfOffBits = GET_4B(bmpfileheader, 10);
    
 
    
   if (!ReadOK(source->pub.input_file, bmpinfoheader, 4))
     ERREXIT(cinfo, JERR_INPUT_EOF);
   headerSize = GET_4B(bmpinfoheader, 0);
   if (headerSize < 12 || headerSize > 64)
     ERREXIT(cinfo, JERR_BMP_BADHEADER);
   if (!ReadOK(source->pub.input_file, bmpinfoheader + 4, headerSize - 4))
     ERREXIT(cinfo, JERR_INPUT_EOF);
 
   switch (headerSize) {
   case 12:
      
     biWidth = (int)GET_2B(bmpinfoheader, 4);
     biHeight = (int)GET_2B(bmpinfoheader, 6);
     biPlanes = GET_2B(bmpinfoheader, 8);
     source->bits_per_pixel = (int)GET_2B(bmpinfoheader, 10);
 
     switch (source->bits_per_pixel) {
     case 8:                      
       mapentrysize = 3;          
       TRACEMS2(cinfo, 1, JTRC_BMP_OS2_MAPPED, biWidth, biHeight);
       break;
     case 24:                     
       TRACEMS2(cinfo, 1, JTRC_BMP_OS2, biWidth, biHeight);
       break;
     default:
       ERREXIT(cinfo, JERR_BMP_BADDEPTH);
       break;
     }
     break;
   case 40:
   case 64:
      
      
     biWidth = (int)GET_4B(bmpinfoheader, 4);
     biHeight = (int)GET_4B(bmpinfoheader, 8);
     biPlanes = GET_2B(bmpinfoheader, 12);
     source->bits_per_pixel = (int)GET_2B(bmpinfoheader, 14);
     biCompression = GET_4B(bmpinfoheader, 16);
     biXPelsPerMeter = (int)GET_4B(bmpinfoheader, 24);
     biYPelsPerMeter = (int)GET_4B(bmpinfoheader, 28);
     biClrUsed = GET_4B(bmpinfoheader, 32);
      
 
     switch (source->bits_per_pixel) {
     case 8:                      
       mapentrysize = 4;          
       TRACEMS2(cinfo, 1, JTRC_BMP_MAPPED, biWidth, biHeight);
       break;
     case 24:                     
       TRACEMS2(cinfo, 1, JTRC_BMP, biWidth, biHeight);
       break;
     case 32:                     
       TRACEMS2(cinfo, 1, JTRC_BMP, biWidth, biHeight);
       break;
     default:
       ERREXIT(cinfo, JERR_BMP_BADDEPTH);
       break;
     }
     if (biCompression != 0)
       ERREXIT(cinfo, JERR_BMP_COMPRESSED);
 
     if (biXPelsPerMeter > 0 && biYPelsPerMeter > 0) {
        
       cinfo->X_density = (UINT16)(biXPelsPerMeter / 100);  
       cinfo->Y_density = (UINT16)(biYPelsPerMeter / 100);
       cinfo->density_unit = 2;   
     }
     break;
   default:
     ERREXIT(cinfo, JERR_BMP_BADHEADER);
     return;
   }
 
   if (biWidth <= 0 || biHeight <= 0)
     ERREXIT(cinfo, JERR_BMP_EMPTY);
   if (biPlanes != 1)
     ERREXIT(cinfo, JERR_BMP_BADPLANES);
 
    
   bPad = bfOffBits - (headerSize + 14);
 
    
   if (mapentrysize > 0) {
     if (biClrUsed <= 0)
       biClrUsed = 256;           
     else if (biClrUsed > 256)
       ERREXIT(cinfo, JERR_BMP_BADCMAP);
      
     source->colormap = (*cinfo->mem->alloc_sarray)
       ((j_common_ptr)cinfo, JPOOL_IMAGE, (JDIMENSION)biClrUsed, (JDIMENSION)3);
      
     read_colormap(source, (int)biClrUsed, mapentrysize);
      
     bPad -= biClrUsed * mapentrysize;
   }
 
    
   if (bPad < 0)                  
     ERREXIT(cinfo, JERR_BMP_BADHEADER);
   while (--bPad >= 0) {
     (void)read_byte(source);
   }
 
    
   switch (source->bits_per_pixel) {
   case 8:
     if (cinfo->in_color_space == JCS_UNKNOWN)
       cinfo->in_color_space = JCS_EXT_RGB;
     if (IsExtRGB(cinfo->in_color_space))
       cinfo->input_components = rgb_pixelsize[cinfo->in_color_space];
     else if (cinfo->in_color_space == JCS_GRAYSCALE)
       cinfo->input_components = 1;
     else if (cinfo->in_color_space == JCS_CMYK)
       cinfo->input_components = 4;
     else
       ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
     row_width = (JDIMENSION)biWidth;
     break;
   case 24:
     if (cinfo->in_color_space == JCS_UNKNOWN)
       cinfo->in_color_space = JCS_EXT_BGR;
     if (IsExtRGB(cinfo->in_color_space))
       cinfo->input_components = rgb_pixelsize[cinfo->in_color_space];
     else if (cinfo->in_color_space == JCS_CMYK)
       cinfo->input_components = 4;
     else
       ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
     row_width = (JDIMENSION)(biWidth * 3);
     break;
   case 32:
     if (cinfo->in_color_space == JCS_UNKNOWN)
       cinfo->in_color_space = JCS_EXT_BGRA;
     if (IsExtRGB(cinfo->in_color_space))
       cinfo->input_components = rgb_pixelsize[cinfo->in_color_space];
     else if (cinfo->in_color_space == JCS_CMYK)
       cinfo->input_components = 4;
     else
       ERREXIT(cinfo, JERR_BAD_IN_COLORSPACE);
     row_width = (JDIMENSION)(biWidth * 4);
     break;
   default:
     ERREXIT(cinfo, JERR_BMP_BADDEPTH);
   }
   while ((row_width & 3) != 0) row_width++;
   source->row_width = row_width;
 
   if (source->use_inversion_array) {
      
     source->whole_image = (*cinfo->mem->request_virt_sarray)
       ((j_common_ptr)cinfo, JPOOL_IMAGE, FALSE,
        row_width, (JDIMENSION)biHeight, (JDIMENSION)1);
     source->pub.get_pixel_rows = preload_image;
     if (cinfo->progress != NULL) {
       cd_progress_ptr progress = (cd_progress_ptr)cinfo->progress;
       progress->total_extra_passes++;  
     }
   } else {
     source->iobuffer = (U_CHAR *)
       (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_IMAGE, row_width);
     switch (source->bits_per_pixel) {
     case 8:
       source->pub.get_pixel_rows = get_8bit_row;
       break;
     case 24:
       source->pub.get_pixel_rows = get_24bit_row;
       break;
     case 32:
       source->pub.get_pixel_rows = get_32bit_row;
       break;
     default:
       ERREXIT(cinfo, JERR_BMP_BADDEPTH);
      }
    }
  
//    
//   if ((unsigned long long)biWidth *
//       (unsigned long long)cinfo->input_components > 0xFFFFFFFFULL)
//     ERREXIT(cinfo, JERR_WIDTH_OVERFLOW);
     
    source->pub.buffer = (*cinfo->mem->alloc_sarray)
      ((j_common_ptr)cinfo, JPOOL_IMAGE,
      (JDIMENSION)(biWidth * cinfo->input_components), (JDIMENSION)1);
   source->pub.buffer_height = 1;
 
   cinfo->data_precision = 8;
   cinfo->image_width = (JDIMENSION)biWidth;
   cinfo->image_height = (JDIMENSION)biHeight;
 }
 uch *readpng_get_image(double display_exponent, int *pChannels, ulg *pRowbytes)
 {
     ulg  rowbytes;
 
 
   
 
   
 
  *pRowbytes = rowbytes = channels*width;
  *pChannels = channels;
 
  if ((image_data = (uch *)malloc(rowbytes*height)) == NULL) {
  return NULL;
  }
 
  Trace((stderr, "readpng_get_image:  rowbytes = %ld, height = %ld\n", rowbytes, height));
 
 
  
       
  
    fread(image_data, 1L, rowbytes*height, saved_infile);
//     if (fread(image_data, 1L, rowbytes*height, saved_infile) <
//        rowbytes*height) {
//         free (image_data);
//         image_data = NULL;
//         return NULL;
//     }
  
      return image_data;
  }
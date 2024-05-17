 write_one_file(Image *output, Image *image, int convert_to_8bit)
 {
  if (image->opts & FAST_WRITE)
       image->image.flags |= PNG_IMAGE_FLAG_FAST;
 
  
     if (image->opts & USE_STDIO)
     {
// #ifndef PNG_USE_MKSTEMP
        FILE *f = tmpfile();
// #else
//        
//       char tmpfile[] = "pngstest-XXXXXX";
//       int filedes;
//       FILE *f;
//       umask(0177);
//       filedes = mkstemp(tmpfile);
//       if (filedes < 0)
//         f = NULL;
//       else
//       {
//         f = fdopen(filedes,"w+");
//          
//         (void) unlink(tmpfile);
//       }
// #endif
  
        if (f != NULL)
        {
  if (png_image_write_to_stdio(&image->image, f, convert_to_8bit,
             image->buffer+16, (png_int_32)image->stride, image->colormap))
  {
  if (fflush(f) == 0)
  {
                rewind(f);
                initimage(output, image->opts, "tmpfile", image->stride_extra);
                output->input_file = f;
  if (!checkopaque(image))
  return 0;
  }
 
  else
  return logclose(image, f, "tmpfile", ": flush: ");
  }
 
  else
  {
             fclose(f);
  return logerror(image, "tmpfile", ": write failed", "");
  }
  }
 
  else
  return logerror(image, "tmpfile", ": open: ", strerror(errno));
  }
 
  else
  {
  static int counter = 0;
  char name[32];
 
       sprintf(name, "%s%d.png", tmpf, ++counter);
 
  if (png_image_write_to_file(&image->image, name, convert_to_8bit,
          image->buffer+16, (png_int_32)image->stride, image->colormap))
  {
          initimage(output, image->opts, output->tmpfile_name,
             image->stride_extra);
   
          strcpy(output->tmpfile_name, name);
 
  if (!checkopaque(image))
  return 0;
  }
 
  else
  return logerror(image, name, ": write failed", "");
  }
 
   
  if (read_file(output, image->image.format | FORMAT_NO_CHANGE, NULL))
  {
       png_uint_32 original_format = image->image.format;
 
  if (convert_to_8bit)
          original_format &= ~PNG_FORMAT_FLAG_LINEAR;
 
  if ((output->image.format & BASE_FORMATS) !=
  (original_format & BASE_FORMATS))
  return logerror(image, image->file_name, ": format changed on read: ",
             output->file_name);
 
  return compare_two_images(image, output, 0 , NULL);
  }
 
  else
  return logerror(output, output->tmpfile_name,
  ": read of new file failed", "");
 }
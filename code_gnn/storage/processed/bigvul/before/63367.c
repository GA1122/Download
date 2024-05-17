static void JPEGErrorHandler(j_common_ptr jpeg_info)
{
  char
    message[JMSG_LENGTH_MAX];

  ErrorManager
    *error_manager;

  Image
    *image;

  *message='\0';
  error_manager=(ErrorManager *) jpeg_info->client_data;
  image=error_manager->image;
  (jpeg_info->err->format_message)(jpeg_info,message);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(CoderEvent,GetMagickModule(),
      "[%s] JPEG Trace: \"%s\"",image->filename,message);
  if (error_manager->finished != MagickFalse)
    (void) ThrowMagickException(&image->exception,GetMagickModule(),
      CorruptImageWarning,(char *) message,"`%s'",image->filename);
  else
    (void) ThrowMagickException(&image->exception,GetMagickModule(),
      CorruptImageError,(char *) message,"`%s'",image->filename);
  longjmp(error_manager->error_recovery,1);
}

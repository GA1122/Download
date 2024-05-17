static void MSLWarning(void *context,const char *format,...)
{
  char
    *message,
    reason[MagickPathExtent];

  MSLInfo
    *msl_info;

  va_list
    operands;

   
  va_start(operands,format);
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"  SAX.warning: ");
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),format,operands);
  msl_info=(MSLInfo *) context;
  (void) msl_info;
#if !defined(MAGICKCORE_HAVE_VSNPRINTF)
  (void) vsprintf(reason,format,operands);
#else
  (void) vsnprintf(reason,MagickPathExtent,format,operands);
#endif
  message=GetExceptionMessage(errno);
  ThrowMSLException(CoderError,reason,message);
  message=DestroyString(message);
  va_end(operands);
}
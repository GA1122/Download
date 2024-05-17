static void MSLError(void *context,const char *format,...)
{
  char
    reason[MagickPathExtent];

  MSLInfo
    *msl_info;

  va_list
    operands;

   
  va_start(operands,format);
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"  SAX.error: ");
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),format,operands);
  msl_info=(MSLInfo *) context;
  (void) msl_info;
#if !defined(MAGICKCORE_HAVE_VSNPRINTF)
  (void) vsprintf(reason,format,operands);
#else
  (void) vsnprintf(reason,MagickPathExtent,format,operands);
#endif
  ThrowMSLException(DelegateFatalError,reason,"SAX error");
  va_end(operands);
}

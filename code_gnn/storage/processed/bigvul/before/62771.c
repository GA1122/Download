static void MSLComment(void *context,const xmlChar *value)
{
  MSLInfo
    *msl_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.comment(%s)",value);
  msl_info=(MSLInfo *) context;
  (void) msl_info;
}

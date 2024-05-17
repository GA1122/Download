static void MSLEndDocument(void *context)
{
  MSLInfo
    *msl_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"  SAX.endDocument()");
  msl_info=(MSLInfo *) context;
  if (msl_info->content != (char *) NULL)
    msl_info->content=DestroyString(msl_info->content);
}

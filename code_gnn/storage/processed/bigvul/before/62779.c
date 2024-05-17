static xmlEntityPtr MSLGetParameterEntity(void *context,const xmlChar *name)
{
  MSLInfo
    *msl_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.getParameterEntity(%s)",(const char *) name);
  msl_info=(MSLInfo *) context;
  return(xmlGetParameterEntity(msl_info->document,name));
}
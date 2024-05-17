static int MSLIsStandalone(void *context)
{
  MSLInfo
    *msl_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"  SAX.MSLIsStandalone()");
  msl_info=(MSLInfo *) context;
  return(msl_info->document->standalone == 1);
}

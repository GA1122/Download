static int MSLHasExternalSubset(void *context)
{
  MSLInfo
    *msl_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.MSLHasExternalSubset()");
  msl_info=(MSLInfo *) context;
  return(msl_info->document->extSubset != NULL);
}

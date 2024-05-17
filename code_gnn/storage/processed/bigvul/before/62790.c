static xmlParserInputPtr MSLResolveEntity(void *context,
  const xmlChar *public_id,const xmlChar *system_id)
{
  MSLInfo
    *msl_info;

  xmlParserInputPtr
    stream;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.resolveEntity(%s, %s)",
    (public_id != (const xmlChar *) NULL ? (const char *) public_id : "none"),
    (system_id != (const xmlChar *) NULL ? (const char *) system_id : "none"));
  msl_info=(MSLInfo *) context;
  stream=xmlLoadExternalEntity((const char *) system_id,(const char *)
    public_id,msl_info->parser);
  return(stream);
}
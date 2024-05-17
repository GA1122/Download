static MagickBooleanType SetImageProfileInternal(Image *image,const char *name,
  const StringInfo *profile,const MagickBooleanType recursive)
{
  char
    key[MaxTextExtent],
    property[MaxTextExtent];

  MagickBooleanType
    status;

  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if (image->profiles == (SplayTreeInfo *) NULL)
    image->profiles=NewSplayTree(CompareSplayTreeString,RelinquishMagickMemory,
      DestroyProfile);
  (void) CopyMagickString(key,name,MaxTextExtent);
  LocaleLower(key);
  status=AddValueToSplayTree((SplayTreeInfo *) image->profiles,
    ConstantString(key),CloneStringInfo(profile));
  if ((status != MagickFalse) &&
      ((LocaleCompare(name,"icc") == 0) || (LocaleCompare(name,"icm") == 0)))
    {
      const StringInfo
        *icc_profile;

       
      icc_profile=GetImageProfile(image,name);
      if (icc_profile != (const StringInfo *) NULL)
        {
          image->color_profile.length=GetStringInfoLength(icc_profile);
          image->color_profile.info=GetStringInfoDatum(icc_profile);
        }
    }
  if ((status != MagickFalse) &&
      ((LocaleCompare(name,"iptc") == 0) || (LocaleCompare(name,"8bim") == 0)))
    {
      const StringInfo
        *iptc_profile;

       
      iptc_profile=GetImageProfile(image,name);
      if (iptc_profile != (const StringInfo *) NULL)
        {
          image->iptc_profile.length=GetStringInfoLength(iptc_profile);
          image->iptc_profile.info=GetStringInfoDatum(iptc_profile);
        }
    }
  if (status != MagickFalse)
    {
      if (LocaleCompare(name,"8bim") == 0)
        GetProfilesFromResourceBlock(image,profile);
      else if (recursive == MagickFalse)
        WriteTo8BimProfile(image,name,profile);
    }
   
  (void) FormatLocaleString(property,MaxTextExtent,"%s:*",name);
  (void) GetImageProperty(image,property);
  return(status);
}
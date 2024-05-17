static void TagToModuleName(const char *tag,const char *format,char *module)
{
  char
    name[MaxTextExtent];

  assert(tag != (const char *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",tag);
  assert(format != (const char *) NULL);
  assert(module != (char *) NULL);
  (void) CopyMagickString(name,tag,MaxTextExtent);
  LocaleUpper(name);
#if !defined(MAGICKCORE_NAMESPACE_PREFIX)
  (void) FormatLocaleString(module,MaxTextExtent,format,name);
#else
  {
    char
      prefix_format[MaxTextExtent];

    (void) FormatLocaleString(prefix_format,MaxTextExtent,"%s%s",
      MAGICKCORE_NAMESPACE_PREFIX,format);
    (void) FormatLocaleString(module,MaxTextExtent,prefix_format,name);
  }
#endif
}

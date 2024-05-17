static int SVGHasInternalSubset(void *context)
{
  SVGInfo
    *svg_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),
    "  SAX.SVGHasInternalSubset()");
  svg_info=(SVGInfo *) context;
  return(svg_info->document->intSubset != NULL);
}

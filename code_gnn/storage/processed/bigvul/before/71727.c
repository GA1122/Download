static xmlEntityPtr SVGGetEntity(void *context,const xmlChar *name)
{
  SVGInfo
    *svg_info;

   
  (void) LogMagickEvent(CoderEvent,GetMagickModule(),"  SAX.SVGGetEntity(%s)",
    name);
  svg_info=(SVGInfo *) context;
  return(xmlGetDocEntity(svg_info->document,name));
}

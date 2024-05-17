png_get_header_version(png_structp png_ptr)
{
    
   PNG_UNUSED(png_ptr)  
#ifdef __STDC__
   return ((png_charp) PNG_HEADER_VERSION_STRING
#ifndef PNG_READ_SUPPORTED
   "     (NO READ SUPPORT)"
#endif
   PNG_STRING_NEWLINE);
#else
   return ((png_charp) PNG_HEADER_VERSION_STRING);
#endif
}

get_unknown(display *d, png_infop info_ptr, int after_IDAT)
  
{
   UNUSED(info_ptr);

 if (after_IDAT)
 return d->after_IDAT;

 else
 return d->before_IDAT;
}

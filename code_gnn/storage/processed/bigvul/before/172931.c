png_init_filter_functions_neon(png_structp pp, unsigned int bpp)
{
  
#ifdef PNG_ARM_NEON_API_SUPPORTED
 switch ((pp->options >> PNG_ARM_NEON) & 3)
 {
 case PNG_OPTION_UNSET:
  
#endif  
#ifdef PNG_ARM_NEON_CHECK_SUPPORTED
 {
 static volatile sig_atomic_t no_neon = -1;  

 if (no_neon < 0)
               no_neon = !png_have_neon(pp);

 if (no_neon)
 return;
 }
#ifdef PNG_ARM_NEON_API_SUPPORTED
 break;
#endif
#endif  

#ifdef PNG_ARM_NEON_API_SUPPORTED
 default:  
 return;

 case PNG_OPTION_ON:
  
 break;
 }
#endif

  
   pp->read_filter[PNG_FILTER_VALUE_UP-1] = png_read_filter_row_up_neon;

 if (bpp == 3)
 {
      pp->read_filter[PNG_FILTER_VALUE_SUB-1] = png_read_filter_row_sub3_neon;
      pp->read_filter[PNG_FILTER_VALUE_AVG-1] = png_read_filter_row_avg3_neon;
      pp->read_filter[PNG_FILTER_VALUE_PAETH-1] =
         png_read_filter_row_paeth3_neon;
 }

 else if (bpp == 4)
 {
      pp->read_filter[PNG_FILTER_VALUE_SUB-1] = png_read_filter_row_sub4_neon;
      pp->read_filter[PNG_FILTER_VALUE_AVG-1] = png_read_filter_row_avg4_neon;
      pp->read_filter[PNG_FILTER_VALUE_PAETH-1] =
          png_read_filter_row_paeth4_neon;

    }
 }

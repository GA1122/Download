png_write_pCAL(png_structp png_ptr, png_charp purpose, png_int_32 X0,
   png_int_32 X1, int type, int nparams, png_charp units, png_charpp params)
{
#ifdef PNG_USE_LOCAL_ARRAYS
   PNG_pCAL;
#endif
   png_size_t purpose_len, units_len, total_len;
   png_uint_32p params_len;
   png_byte buf[10];
   png_charp new_purpose;
   int i;

   png_debug1(1, "in png_write_pCAL (%d parameters)", nparams);

   if (type >= PNG_EQUATION_LAST)
      png_warning(png_ptr, "Unrecognized equation type for pCAL chunk");

   purpose_len = png_check_keyword(png_ptr, purpose, &new_purpose) + 1;
   png_debug1(3, "pCAL purpose length = %d", (int)purpose_len);
   units_len = png_strlen(units) + (nparams == 0 ? 0 : 1);
   png_debug1(3, "pCAL units length = %d", (int)units_len);
   total_len = purpose_len + units_len + 10;

   params_len = (png_uint_32p)png_malloc(png_ptr,
      (png_uint_32)(nparams * png_sizeof(png_uint_32)));

    
   for (i = 0; i < nparams; i++)
   {
      params_len[i] = png_strlen(params[i]) + (i == nparams - 1 ? 0 : 1);
      png_debug2(3, "pCAL parameter %d length = %lu", i,
        (unsigned long) params_len[i]);
      total_len += (png_size_t)params_len[i];
   }

   png_debug1(3, "pCAL total length = %d", (int)total_len);
   png_write_chunk_start(png_ptr, (png_bytep)png_pCAL, (png_uint_32)total_len);
   png_write_chunk_data(png_ptr, (png_bytep)new_purpose,
     (png_size_t)purpose_len);
   png_save_int_32(buf, X0);
   png_save_int_32(buf + 4, X1);
   buf[8] = (png_byte)type;
   buf[9] = (png_byte)nparams;
   png_write_chunk_data(png_ptr, buf, (png_size_t)10);
   png_write_chunk_data(png_ptr, (png_bytep)units, (png_size_t)units_len);

   png_free(png_ptr, new_purpose);

   for (i = 0; i < nparams; i++)
   {
      png_write_chunk_data(png_ptr, (png_bytep)params[i],
         (png_size_t)params_len[i]);
   }

   png_free(png_ptr, params_len);
   png_write_chunk_end(png_ptr);
}

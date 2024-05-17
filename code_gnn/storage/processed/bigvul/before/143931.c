png_free_data(png_structp png_ptr, png_infop info_ptr, png_uint_32 mask,
   int num)
{
   png_debug(1, "in png_free_data");

   if (png_ptr == NULL || info_ptr == NULL)
      return;

#ifdef PNG_TEXT_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_TEXT) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_TEXT)
#endif
   {
      if (num != -1)
      {
         if (info_ptr->text && info_ptr->text[num].key)
         {
            png_free(png_ptr, info_ptr->text[num].key);
            info_ptr->text[num].key = NULL;
         }
      }
      else
      {
         int i;
         for (i = 0; i < info_ptr->num_text; i++)
             png_free_data(png_ptr, info_ptr, PNG_FREE_TEXT, i);
         png_free(png_ptr, info_ptr->text);
         info_ptr->text = NULL;
         info_ptr->num_text=0;
      }
   }
#endif

#ifdef PNG_tRNS_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_TRNS) & info_ptr->free_me)
#else
   if ((mask & PNG_FREE_TRNS) && (png_ptr->flags & PNG_FLAG_FREE_TRNS))
#endif
   {
      png_free(png_ptr, info_ptr->trans);
      info_ptr->trans = NULL;
      info_ptr->valid &= ~PNG_INFO_tRNS;
#ifndef PNG_FREE_ME_SUPPORTED
      png_ptr->flags &= ~PNG_FLAG_FREE_TRNS;
#endif
   }
#endif

#ifdef PNG_sCAL_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_SCAL) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_SCAL)
#endif
   {
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free(png_ptr, info_ptr->scal_s_width);
      png_free(png_ptr, info_ptr->scal_s_height);
      info_ptr->scal_s_width = NULL;
      info_ptr->scal_s_height = NULL;
#endif
      info_ptr->valid &= ~PNG_INFO_sCAL;
   }
#endif

#ifdef PNG_pCAL_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_PCAL) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_PCAL)
#endif
   {
      png_free(png_ptr, info_ptr->pcal_purpose);
      png_free(png_ptr, info_ptr->pcal_units);
      info_ptr->pcal_purpose = NULL;
      info_ptr->pcal_units = NULL;
      if (info_ptr->pcal_params != NULL)
         {
            int i;
            for (i = 0; i < (int)info_ptr->pcal_nparams; i++)
            {
               png_free(png_ptr, info_ptr->pcal_params[i]);
               info_ptr->pcal_params[i] = NULL;
            }
            png_free(png_ptr, info_ptr->pcal_params);
            info_ptr->pcal_params = NULL;
         }
      info_ptr->valid &= ~PNG_INFO_pCAL;
   }
#endif

#ifdef PNG_iCCP_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_ICCP) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_ICCP)
#endif
   {
      png_free(png_ptr, info_ptr->iccp_name);
      png_free(png_ptr, info_ptr->iccp_profile);
      info_ptr->iccp_name = NULL;
      info_ptr->iccp_profile = NULL;
      info_ptr->valid &= ~PNG_INFO_iCCP;
   }
#endif

#ifdef PNG_sPLT_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_SPLT) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_SPLT)
#endif
   {
      if (num != -1)
      {
         if (info_ptr->splt_palettes)
         {
            png_free(png_ptr, info_ptr->splt_palettes[num].name);
            png_free(png_ptr, info_ptr->splt_palettes[num].entries);
            info_ptr->splt_palettes[num].name = NULL;
            info_ptr->splt_palettes[num].entries = NULL;
         }
      }
      else
      {
         if (info_ptr->splt_palettes_num)
         {
            int i;
            for (i = 0; i < (int)info_ptr->splt_palettes_num; i++)
               png_free_data(png_ptr, info_ptr, PNG_FREE_SPLT, i);

            png_free(png_ptr, info_ptr->splt_palettes);
            info_ptr->splt_palettes = NULL;
            info_ptr->splt_palettes_num = 0;
         }
         info_ptr->valid &= ~PNG_INFO_sPLT;
      }
   }
#endif

#ifdef PNG_UNKNOWN_CHUNKS_SUPPORTED
   if (png_ptr->unknown_chunk.data)
   {
      png_free(png_ptr, png_ptr->unknown_chunk.data);
      png_ptr->unknown_chunk.data = NULL;
   }

#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_UNKN) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_UNKN)
#endif
   {
      if (num != -1)
      {
          if (info_ptr->unknown_chunks)
          {
             png_free(png_ptr, info_ptr->unknown_chunks[num].data);
             info_ptr->unknown_chunks[num].data = NULL;
          }
      }
      else
      {
         int i;

         if (info_ptr->unknown_chunks_num)
         {
            for (i = 0; i < (int)info_ptr->unknown_chunks_num; i++)
               png_free_data(png_ptr, info_ptr, PNG_FREE_UNKN, i);

            png_free(png_ptr, info_ptr->unknown_chunks);
            info_ptr->unknown_chunks = NULL;
            info_ptr->unknown_chunks_num = 0;
         }
      }
   }
#endif

#ifdef PNG_hIST_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_HIST)  & info_ptr->free_me)
#else
   if ((mask & PNG_FREE_HIST) && (png_ptr->flags & PNG_FLAG_FREE_HIST))
#endif
   {
      png_free(png_ptr, info_ptr->hist);
      info_ptr->hist = NULL;
      info_ptr->valid &= ~PNG_INFO_hIST;
#ifndef PNG_FREE_ME_SUPPORTED
      png_ptr->flags &= ~PNG_FLAG_FREE_HIST;
#endif
   }
#endif

    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_PLTE) & info_ptr->free_me)
#else
   if ((mask & PNG_FREE_PLTE) && (png_ptr->flags & PNG_FLAG_FREE_PLTE))
#endif
   {
      png_zfree(png_ptr, info_ptr->palette);
      info_ptr->palette = NULL;
      info_ptr->valid &= ~PNG_INFO_PLTE;
#ifndef PNG_FREE_ME_SUPPORTED
      png_ptr->flags &= ~PNG_FLAG_FREE_PLTE;
#endif
      info_ptr->num_palette = 0;
   }

#ifdef PNG_INFO_IMAGE_SUPPORTED
    
#ifdef PNG_FREE_ME_SUPPORTED
   if ((mask & PNG_FREE_ROWS) & info_ptr->free_me)
#else
   if (mask & PNG_FREE_ROWS)
#endif
   {
      if (info_ptr->row_pointers)
      {
         int row;
         for (row = 0; row < (int)info_ptr->height; row++)
         {
            png_free(png_ptr, info_ptr->row_pointers[row]);
            info_ptr->row_pointers[row] = NULL;
         }
         png_free(png_ptr, info_ptr->row_pointers);
         info_ptr->row_pointers = NULL;
      }
      info_ptr->valid &= ~PNG_INFO_IDAT;
   }
#endif

#ifdef PNG_FREE_ME_SUPPORTED
   if (num == -1)
      info_ptr->free_me &= ~mask;
   else
      info_ptr->free_me &= ~(mask & ~PNG_FREE_MUL);
#endif
}

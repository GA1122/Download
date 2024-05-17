png_correct_palette(png_structp png_ptr, png_colorp palette,
   int num_palette)
{
   png_debug(1, "in png_correct_palette");

#if defined(PNG_READ_BACKGROUND_SUPPORTED) && \
    defined(PNG_READ_GAMMA_SUPPORTED) && \
  defined(PNG_FLOATING_POINT_SUPPORTED)
   if (png_ptr->transformations & (PNG_GAMMA | PNG_BACKGROUND))
   {
      png_color back, back_1;

      if (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE)
      {
         back.red = png_ptr->gamma_table[png_ptr->background.red];
         back.green = png_ptr->gamma_table[png_ptr->background.green];
         back.blue = png_ptr->gamma_table[png_ptr->background.blue];

         back_1.red = png_ptr->gamma_to_1[png_ptr->background.red];
         back_1.green = png_ptr->gamma_to_1[png_ptr->background.green];
         back_1.blue = png_ptr->gamma_to_1[png_ptr->background.blue];
      }
      else
      {
         double g;

         g = 1.0 / (png_ptr->background_gamma * png_ptr->screen_gamma);

         if (png_ptr->background_gamma_type == PNG_BACKGROUND_GAMMA_SCREEN
             || fabs(g - 1.0) < PNG_GAMMA_THRESHOLD)
         {
            back.red = png_ptr->background.red;
            back.green = png_ptr->background.green;
            back.blue = png_ptr->background.blue;
         }
         else
         {
            back.red =
               (png_byte)(pow((double)png_ptr->background.red/255, g) *
                255.0 + 0.5);
            back.green =
               (png_byte)(pow((double)png_ptr->background.green/255, g) *
                255.0 + 0.5);
            back.blue =
               (png_byte)(pow((double)png_ptr->background.blue/255, g) *
                255.0 + 0.5);
         }

         g = 1.0 / png_ptr->background_gamma;

         back_1.red =
            (png_byte)(pow((double)png_ptr->background.red/255, g) *
             255.0 + 0.5);
         back_1.green =
            (png_byte)(pow((double)png_ptr->background.green/255, g) *
             255.0 + 0.5);
         back_1.blue =
            (png_byte)(pow((double)png_ptr->background.blue/255, g) *
             255.0 + 0.5);
      }

      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_uint_32 i;

         for (i = 0; i < (png_uint_32)num_palette; i++)
         {
            if (i < png_ptr->num_trans && png_ptr->trans[i] == 0)
            {
               palette[i] = back;
            }
            else if (i < png_ptr->num_trans && png_ptr->trans[i] != 0xff)
            {
               png_byte v, w;

               v = png_ptr->gamma_to_1[png_ptr->palette[i].red];
               png_composite(w, v, png_ptr->trans[i], back_1.red);
               palette[i].red = png_ptr->gamma_from_1[w];

               v = png_ptr->gamma_to_1[png_ptr->palette[i].green];
               png_composite(w, v, png_ptr->trans[i], back_1.green);
               palette[i].green = png_ptr->gamma_from_1[w];

               v = png_ptr->gamma_to_1[png_ptr->palette[i].blue];
               png_composite(w, v, png_ptr->trans[i], back_1.blue);
               palette[i].blue = png_ptr->gamma_from_1[w];
            }
            else
            {
               palette[i].red = png_ptr->gamma_table[palette[i].red];
               palette[i].green = png_ptr->gamma_table[palette[i].green];
               palette[i].blue = png_ptr->gamma_table[palette[i].blue];
            }
         }
      }
      else
      {
         int i;

         for (i = 0; i < num_palette; i++)
         {
            if (palette[i].red == (png_byte)png_ptr->trans_values.gray)
            {
               palette[i] = back;
            }
            else
            {
               palette[i].red = png_ptr->gamma_table[palette[i].red];
               palette[i].green = png_ptr->gamma_table[palette[i].green];
               palette[i].blue = png_ptr->gamma_table[palette[i].blue];
            }
         }
      }
   }
   else
#endif
#ifdef PNG_READ_GAMMA_SUPPORTED
   if (png_ptr->transformations & PNG_GAMMA)
   {
      int i;

      for (i = 0; i < num_palette; i++)
      {
         palette[i].red = png_ptr->gamma_table[palette[i].red];
         palette[i].green = png_ptr->gamma_table[palette[i].green];
         palette[i].blue = png_ptr->gamma_table[palette[i].blue];
      }
   }
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   else
#endif
#endif
#ifdef PNG_READ_BACKGROUND_SUPPORTED
   if (png_ptr->transformations & PNG_BACKGROUND)
   {
      if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      {
         png_color back;

         back.red   = (png_byte)png_ptr->background.red;
         back.green = (png_byte)png_ptr->background.green;
         back.blue  = (png_byte)png_ptr->background.blue;

         for (i = 0; i < (int)png_ptr->num_trans; i++)
         {
            if (png_ptr->trans[i] == 0)
            {
               palette[i].red = back.red;
               palette[i].green = back.green;
               palette[i].blue = back.blue;
            }
            else if (png_ptr->trans[i] != 0xff)
            {
               png_composite(palette[i].red, png_ptr->palette[i].red,
                  png_ptr->trans[i], back.red);
               png_composite(palette[i].green, png_ptr->palette[i].green,
                  png_ptr->trans[i], back.green);
               png_composite(palette[i].blue, png_ptr->palette[i].blue,
                  png_ptr->trans[i], back.blue);
            }
         }
      }
      else  
      {
         int i;

         for (i = 0; i < num_palette; i++)
         {
            if (i == (png_byte)png_ptr->trans_values.gray)
            {
               palette[i].red = (png_byte)png_ptr->background.red;
               palette[i].green = (png_byte)png_ptr->background.green;
               palette[i].blue = (png_byte)png_ptr->background.blue;
            }
         }
      }
   }
#endif
}

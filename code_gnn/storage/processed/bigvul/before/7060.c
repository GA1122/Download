  premultiply_data( png_structp    png,
                    png_row_infop  row_info,
                    png_bytep      data )
  {
    unsigned int  i;

    FT_UNUSED( png );


    for ( i = 0; i < row_info->rowbytes; i += 4 )
    {
      unsigned char*  base  = &data[i];
      unsigned int    alpha = base[3];


      if ( alpha == 0 )
        base[0] = base[1] = base[2] = base[3] = 0;

      else
      {
        unsigned int  red   = base[0];
        unsigned int  green = base[1];
        unsigned int  blue  = base[2];


        if ( alpha != 0xFF )
        {
          red   = multiply_alpha( alpha, red   );
          green = multiply_alpha( alpha, green );
          blue  = multiply_alpha( alpha, blue  );
        }

        base[0] = blue;
        base[1] = green;
        base[2] = red;
        base[3] = alpha;
      }
    }
  }

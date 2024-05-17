static unsigned preProcessScanlines(unsigned char** out, size_t* outsize, const unsigned char* in,
                                    unsigned w, unsigned h,
                                    const LodePNGInfo* info_png, const LodePNGEncoderSettings* settings)
{
   
  unsigned bpp = lodepng_get_bpp(&info_png->color);
  unsigned error = 0;

  if(info_png->interlace_method == 0)
  {
    *outsize = h + (h * ((w * bpp + 7) / 8));  
    *out = (unsigned char*)calloc(*outsize, 1);
    if(!(*out) && (*outsize)) error = 83;  

    if(!error)
    {
       
      if(bpp < 8 && w * bpp != ((w * bpp + 7) / 8) * 8)
      {
        unsigned char* padded = (unsigned char*)calloc(h * ((w * bpp + 7) / 8), 1);
        if(!padded) error = 83;  
        if(!error)
        {
          addPaddingBits(padded, in, ((w * bpp + 7) / 8) * 8, w * bpp, h);
          error = filter(*out, padded, w, h, &info_png->color, settings);
        }
        free(padded);
      }
      else
      {
         
        error = filter(*out, in, w, h, &info_png->color, settings);
      }
    }
  }
  else  
  {
    unsigned passw[7], passh[7];
    size_t filter_passstart[8], padded_passstart[8], passstart[8];
    unsigned char* adam7;

    Adam7_getpassvalues(passw, passh, filter_passstart, padded_passstart, passstart, w, h, bpp);

    *outsize = filter_passstart[7];  
    *out = (unsigned char*)calloc(*outsize, 1);
    if(!(*out)) error = 83;  

    adam7 = (unsigned char*)calloc(passstart[7], sizeof(unsigned char));
    if(!adam7 && passstart[7]) error = 83;  

    if(!error)
    {
      unsigned i;

      Adam7_interlace(adam7, in, w, h, bpp);
      for(i = 0; i < 7; i++)
      {
        if(bpp < 8)
        {
          unsigned char* padded = (unsigned char*)calloc(padded_passstart[i + 1] - padded_passstart[i], sizeof(unsigned char));
          if(!padded) ERROR_BREAK(83);  
          addPaddingBits(padded, &adam7[passstart[i]],
                         ((passw[i] * bpp + 7) / 8) * 8, passw[i] * bpp, passh[i]);
          error = filter(&(*out)[filter_passstart[i]], padded,
                         passw[i], passh[i], &info_png->color, settings);
          free(padded);
        }
        else
        {
          error = filter(&(*out)[filter_passstart[i]], &adam7[padded_passstart[i]],
                         passw[i], passh[i], &info_png->color, settings);
        }

        if(error) break;
      }
    }

    free(adam7);
  }

  return error;
}

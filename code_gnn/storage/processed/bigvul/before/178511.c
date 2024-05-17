 load(ImlibImage * im, ImlibProgressFunction progress,
      char progress_granularity, char immediate_load)
 {
    int                 rc;
    char                p = ' ', numbers = 3, count = 0;
    int                 w = 0, h = 0, v = 255, c = 0;
    char                buf[256];
    FILE               *f = NULL;
 
    if (im->data)
       return 0;
    f = fopen(im->real_file, "rb");
    if (!f)
       return 0;
 
     
 
     
 
    rc = 0;                       
 
    c = fgetc(f);
    if (c != 'P')
       goto quit;
 
    p = fgetc(f);
    if (p == '1' || p == '4')
       numbers = 2;               
 
    if ((p < '1') || (p > '8'))
       goto quit;
 
    count = 0;
    while (count < numbers)
      {
         c = fgetc(f);
 
         if (c == EOF)
            goto quit;
 
          
         while (isspace(c))
            c = fgetc(f);
          
         if (c == '#')
           {
              do
                 c = fgetc(f);
              while (c != '\n' && c != EOF);
           }
          
         else
           {
              int                 i = 0;
 
               
              while (c != EOF && !isspace(c) && (i < 255))
                {
                   buf[i++] = c;
                   c = fgetc(f);
                }
              if (i)
                {
                   buf[i] = 0;
                   count++;
                   switch (count)
                     {
                         
                     case 1:
                        w = atoi(buf);
                        break;
                         
                     case 2:
                        h = atoi(buf);
                        break;
                         
                     case 3:
                        v = atoi(buf);
                        break;
                     }
                }
           }
      }
    if ((v < 0) || (v > 255))
       goto quit;
 
    im->w = w;
    im->h = h;
    if (!IMAGE_DIMENSIONS_OK(w, h))
       goto quit;
 
    if (!im->format)
      {
         if (p == '8')
            SET_FLAG(im->flags, F_HAS_ALPHA);
         else
            UNSET_FLAG(im->flags, F_HAS_ALPHA);
         im->format = strdup("pnm");
      }
 
    rc = 1;                       
 
    if (((!im->data) && (im->loader)) || (immediate_load) || (progress))
      {
         DATA8              *data = NULL;         
         DATA8              *ptr = NULL;
         int                *idata = NULL;        
         int                *iptr;
         char                buf2[256];
         DATA32             *ptr2;
         int                 i, j, x, y, pl = 0;
         char                pper = 0;
 
          
         ptr2 = im->data = malloc(w * h * sizeof(DATA32));
         if (!im->data)
            goto quit_error;
 
          
         switch (p)
           {
           case '1':             
              buf[0] = 0;
              i = 0;
              for (y = 0; y < h; y++)
                {
                   x = 0;
                   while (x < w)
                     {
                        if (!buf[i])      
                          {
                             if (!fgets(buf, 255, f))
                                goto quit_error;
                             i = 0;
                          }
                        while (buf[i] && isspace(buf[i]))
                           i++;
                        if (buf[i])
                          {
                             if (buf[i] == '1')
                                *ptr2 = 0xff000000;
                             else if (buf[i] == '0')
                                *ptr2 = 0xffffffff;
                             else
                                goto quit_error;
                             ptr2++;
                             i++;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '2':             
              idata = malloc(sizeof(int) * w);
              if (!idata)
                 goto quit_error;
 
              buf[0] = 0;
              i = 0;
              j = 0;
              for (y = 0; y < h; y++)
                {
                   iptr = idata;
                   x = 0;
                   while (x < w)
                     {
                        int                 k;
 
                         
                        for (k = 0; k < 4; k++)
                          {
                             if (!buf[i + k])     
                               {
                                  if (fseek(f, -k, SEEK_CUR) == -1 ||
                                      !fgets(buf, 255, f))
                                     goto quit_error;
                                  i = 0;
                                  break;
                               }
                          }
                        while (buf[i] && isspace(buf[i]))
                           i++;
                        while (buf[i] && !isspace(buf[i]))
                           buf2[j++] = buf[i++];
                        if (j)
                          {
                             buf2[j] = 0;
                             *(iptr++) = atoi(buf2);
                             j = 0;
                             x++;
                           }
                      }
                    iptr = idata;
                  if (v == 255)
//                   if (v == 0 || v == 255)
                      {
                         for (x = 0; x < w; x++)
                           {
                             *ptr2 =
                                0xff000000 | (iptr[0] << 16) | (iptr[0] << 8)
                                | iptr[0];
                             ptr2++;
                             iptr++;
                          }
                     }
                   else
                     {
                        for (x = 0; x < w; x++)
                          {
                             *ptr2 =
                                0xff000000 |
                                (((iptr[0] * 255) / v) << 16) |
                                (((iptr[0] * 255) / v) << 8) |
                                ((iptr[0] * 255) / v);
                             ptr2++;
                             iptr++;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '3':             
              idata = malloc(3 * sizeof(int) * w);
              if (!idata)
                 goto quit_error;
 
              buf[0] = 0;
              i = 0;
              j = 0;
              for (y = 0; y < h; y++)
                {
                   int                 w3 = 3 * w;
 
                   iptr = idata;
                   x = 0;
                   while (x < w3)
                     {
                        int                 k;
 
                         
                        for (k = 0; k < 4; k++)
                          {
                             if (!buf[i + k])     
                               {
                                  if (fseek(f, -k, SEEK_CUR) == -1 ||
                                      !fgets(buf, 255, f))
                                     goto quit_error;
                                  i = 0;
                                  break;
                               }
                          }
                        while (buf[i] && isspace(buf[i]))
                           i++;
                        while (buf[i] && !isspace(buf[i]))
                           buf2[j++] = buf[i++];
                        if (j)
                          {
                             buf2[j] = 0;
                             *(iptr++) = atoi(buf2);
                             j = 0;
                             x++;
                           }
                      }
                    iptr = idata;
                  if (v == 255)
//                   if (v == 0 || v == 255)
                      {
                         for (x = 0; x < w; x++)
                           {
                             *ptr2 =
                                0xff000000 | (iptr[0] << 16) | (iptr[1] << 8)
                                | iptr[2];
                             ptr2++;
                             iptr += 3;
                          }
                     }
                   else
                     {
                        for (x = 0; x < w; x++)
                          {
                             *ptr2 =
                                0xff000000 |
                                (((iptr[0] * 255) / v) << 16) |
                                (((iptr[1] * 255) / v) << 8) |
                                ((iptr[2] * 255) / v);
                             ptr2++;
                             iptr += 3;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '4':             
              data = malloc((w + 7) / 8 * sizeof(DATA8));
              if (!data)
                 goto quit_error;
 
              ptr2 = im->data;
              for (y = 0; y < h; y++)
                {
                   if (!fread(data, (w + 7) / 8, 1, f))
                      goto quit_error;
 
                   ptr = data;
                   for (x = 0; x < w; x += 8)
                     {
                        j = (w - x >= 8) ? 8 : w - x;
                        for (i = 0; i < j; i++)
                          {
                             if (ptr[0] & (0x80 >> i))
                                *ptr2 = 0xff000000;
                             else
                                *ptr2 = 0xffffffff;
                             ptr2++;
                          }
                        ptr++;
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '5':             
              data = malloc(1 * sizeof(DATA8) * w);
              if (!data)
                 goto quit_error;
 
              ptr2 = im->data;
              for (y = 0; y < h; y++)
                {
                   if (!fread(data, w * 1, 1, f))
                       break;
  
                    ptr = data;
                  if (v == 255)
//                   if (v == 0 || v == 255)
                      {
                         for (x = 0; x < w; x++)
                           {
                             *ptr2 =
                                0xff000000 | (ptr[0] << 16) | (ptr[0] << 8) |
                                ptr[0];
                             ptr2++;
                             ptr++;
                          }
                     }
                   else
                     {
                        for (x = 0; x < w; x++)
                          {
                             *ptr2 =
                                0xff000000 |
                                (((ptr[0] * 255) / v) << 16) |
                                (((ptr[0] * 255) / v) << 8) |
                                ((ptr[0] * 255) / v);
                             ptr2++;
                             ptr++;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '6':             
              data = malloc(3 * sizeof(DATA8) * w);
              if (!data)
                 goto quit_error;
 
              ptr2 = im->data;
              for (y = 0; y < h; y++)
                {
                   if (!fread(data, w * 3, 1, f))
                       break;
  
                    ptr = data;
                  if (v == 255)
//                   if (v == 0 || v == 255)
                      {
                         for (x = 0; x < w; x++)
                           {
                             *ptr2 =
                                0xff000000 | (ptr[0] << 16) | (ptr[1] << 8) |
                                ptr[2];
                             ptr2++;
                             ptr += 3;
                          }
                     }
                   else
                     {
                        for (x = 0; x < w; x++)
                          {
                             *ptr2 =
                                0xff000000 |
                                (((ptr[0] * 255) / v) << 16) |
                                (((ptr[1] * 255) / v) << 8) |
                                ((ptr[2] * 255) / v);
                             ptr2++;
                             ptr += 3;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '7':             
              data = malloc(1 * sizeof(DATA8) * w);
              if (!data)
                 goto quit_error;
 
              ptr2 = im->data;
              for (y = 0; y < h; y++)
                {
                   if (!fread(data, w * 1, 1, f))
                      break;
 
                   ptr = data;
                   for (x = 0; x < w; x++)
                     {
                        int                 r, g, b;
 
                        r = (*ptr >> 5) & 0x7;
                        g = (*ptr >> 2) & 0x7;
                        b = (*ptr) & 0x3;
                        *ptr2 =
                           0xff000000 |
                           (((r << 21) | (r << 18) | (r << 15)) & 0xff0000) |
                           (((g << 13) | (g << 10) | (g << 7)) & 0xff00) |
                           ((b << 6) | (b << 4) | (b << 2) | (b << 0));
                        ptr2++;
                        ptr++;
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           case '8':             
              data = malloc(4 * sizeof(DATA8) * w);
              if (!data)
                 goto quit_error;
 
              ptr2 = im->data;
              for (y = 0; y < h; y++)
                {
                   if (!fread(data, w * 4, 1, f))
                       break;
  
                    ptr = data;
                  if (v == 255)
//                   if (v == 0 || v == 255)
                      {
                         for (x = 0; x < w; x++)
                           {
                             *ptr2 =
                                (ptr[3] << 24) | (ptr[0] << 16) |
                                (ptr[1] << 8) | ptr[2];
                             ptr2++;
                             ptr += 4;
                          }
                     }
                   else
                     {
                        for (x = 0; x < w; x++)
                          {
                             *ptr2 =
                                (((ptr[3] * 255) / v) << 24) |
                                (((ptr[0] * 255) / v) << 16) |
                                (((ptr[1] * 255) / v) << 8) |
                                ((ptr[2] * 255) / v);
                             ptr2++;
                             ptr += 4;
                          }
                     }
                   if (progress &&
                       do_progress(im, progress, progress_granularity,
                                   &pper, &pl, y))
                      goto quit_progress;
                }
              break;
           default:
            quit_error:
              rc = 0;
              break;
            quit_progress:
              rc = 2;
              break;
           }
         if (idata)
            free(idata);
         if (data)
            free(data);
      }
  quit:
    fclose(f);
    return rc;
 }
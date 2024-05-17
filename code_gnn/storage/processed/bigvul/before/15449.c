do_progress(ImlibImage * im, ImlibProgressFunction progress,
            char progress_granularity, char *pper, int *py, int y)
{
   int                 rc = 0;
   int                 h = 0;
   char                per;

   per = (char)((100 * y) / im->h);
   if (((per - *pper) >= progress_granularity) || (y == (im->h - 1)))
     {
        h = y - *py;

         
        if (y == (im->h - 1))
           h++;

        rc = !progress(im, per, 0, *py, im->w, h);
        *pper = per;
        *py = y;
     }

   return rc;
}

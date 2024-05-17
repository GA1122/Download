 get_dp(png_structp pp)
     
{
 struct display *dp = (struct display*)png_get_error_ptr(pp);

 if (dp == NULL)
 {
      fprintf(stderr, "pngimage: internal error (no display)\n");
      exit(99);  
 }

 return dp;
}

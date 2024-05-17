png_set_rgb_to_gray(png_structp png_ptr, int error_action, double red,
   double green)
{
   int red_fixed, green_fixed;
   if (png_ptr == NULL)
      return;
   if (red > 21474.83647 || red < -21474.83648 ||
       green > 21474.83647 || green < -21474.83648)
   {
      png_warning(png_ptr, "ignoring out of range rgb_to_gray coefficients");
      red_fixed = -1;
      green_fixed = -1;
   }
   else
   {
      red_fixed = (int)((float)red*100000.0 + 0.5);
      green_fixed = (int)((float)green*100000.0 + 0.5);
   }
   png_set_rgb_to_gray_fixed(png_ptr, error_action, red_fixed, green_fixed);
}

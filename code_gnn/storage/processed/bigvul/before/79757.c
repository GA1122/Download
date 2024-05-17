png_read_filter_row(png_structrp pp, png_row_infop row_info, png_bytep row,
    png_const_bytep prev_row, int filter)
{
    
   if (filter > PNG_FILTER_VALUE_NONE && filter < PNG_FILTER_VALUE_LAST)
   {
      if (pp->read_filter[0] == NULL)
         png_init_filter_functions(pp);

      pp->read_filter[filter-1](row_info, row, prev_row);
   }
}

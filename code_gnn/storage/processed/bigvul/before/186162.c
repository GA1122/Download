 png_write_find_filter(png_structp png_ptr, png_row_infop row_info)
 {
    png_bytep best_row;
 #ifdef PNG_WRITE_FILTER_SUPPORTED
    png_bytep prev_row, row_buf;
     png_uint_32 mins, bpp;
     png_byte filter_to_do = png_ptr->do_filter;
     png_uint_32 row_bytes = row_info->rowbytes;
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
   int num_p_filters = (int)png_ptr->num_prev_filters;
#endif 
  
     png_debug(1, "in png_write_find_filter");
  
#ifndef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
  if (png_ptr->row_number == 0 && filter_to_do == PNG_ALL_FILTERS)
  {
       
      filter_to_do &= ~(PNG_FILTER_UP | PNG_FILTER_PAETH);
  }
#endif 
      
     bpp = (row_info->pixel_depth + 7) >> 3;
  
    prev_row = png_ptr->prev_row;
 #endif
    best_row = png_ptr->row_buf;
 #ifdef PNG_WRITE_FILTER_SUPPORTED
    row_buf = best_row;
    mins = PNG_MAXSUM;
 
     
 
 
     
    if ((filter_to_do & PNG_FILTER_NONE) &&
        filter_to_do != PNG_FILTER_NONE)
    {
       png_bytep rp;
       png_uint_32 sum = 0;
       png_uint_32 i;
       int v;
 
       for (i = 0, rp = row_buf + 1; i < row_bytes; i++, rp++)
       {
          v = *rp;
           sum += (v < 128) ? v : 256 - v;
        }
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         png_uint_32 sumhi, sumlo;
         int j;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;  
          
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_NONE)
            {
               sumlo = (sumlo * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               sumhi = (sumhi * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
          
         sumlo = (sumlo * png_ptr->filter_costs[PNG_FILTER_VALUE_NONE]) >>
            PNG_COST_SHIFT;
         sumhi = (sumhi * png_ptr->filter_costs[PNG_FILTER_VALUE_NONE]) >>
            PNG_COST_SHIFT;
         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;
         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
        mins = sum;
     }
  
     
    if (filter_to_do == PNG_FILTER_SUB)
     
    {
       png_bytep rp, lp, dp;
       png_uint_32 i;
       for (i = 0, rp = row_buf + 1, dp = png_ptr->sub_row + 1; i < bpp;
            i++, rp++, dp++)
       {
          *dp = *rp;
       }
       for (lp = row_buf + 1; i < row_bytes;
          i++, rp++, lp++, dp++)
       {
          *dp = (png_byte)(((int)*rp - (int)*lp) & 0xff);
       }
       best_row = png_ptr->sub_row;
    }
 
    else if (filter_to_do & PNG_FILTER_SUB)
    {
       png_bytep rp, dp, lp;
       png_uint_32 sum = 0, lmins = mins;
        png_uint_32 i;
        int v;
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
       
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_SUB)
            {
               lmlo = (lmlo * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               lmhi = (lmhi * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         lmlo = (lmlo * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
            PNG_COST_SHIFT;
         lmhi = (lmhi * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
            PNG_COST_SHIFT;
         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;
         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif
        for (i = 0, rp = row_buf + 1, dp = png_ptr->sub_row + 1; i < bpp;
             i++, rp++, dp++)
        {
          v = *dp = *rp;
 
          sum += (v < 128) ? v : 256 - v;
       }
       for (lp = row_buf + 1; i < row_bytes;
          i++, rp++, lp++, dp++)
       {
          v = *dp = (png_byte)(((int)*rp - (int)*lp) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
 
          if (sum > lmins)   
              break;
        }
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_SUB)
            {
               sumlo = (sumlo * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               sumhi = (sumhi * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         sumlo = (sumlo * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
            PNG_COST_SHIFT;
         sumhi = (sumhi * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_SUB]) >>
            PNG_COST_SHIFT;
         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;
         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
        if (sum < mins)
        {
           mins = sum;
          best_row = png_ptr->sub_row;
       }
    }
 
     
    if (filter_to_do == PNG_FILTER_UP)
    {
       png_bytep rp, dp, pp;
       png_uint_32 i;
 
       for (i = 0, rp = row_buf + 1, dp = png_ptr->up_row + 1,
            pp = prev_row + 1; i < row_bytes;
            i++, rp++, pp++, dp++)
       {
          *dp = (png_byte)(((int)*rp - (int)*pp) & 0xff);
       }
       best_row = png_ptr->up_row;
    }
 
    else if (filter_to_do & PNG_FILTER_UP)
    {
       png_bytep rp, dp, pp;
       png_uint_32 sum = 0, lmins = mins;
        png_uint_32 i;
        int v;
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_UP)
            {
               lmlo = (lmlo * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               lmhi = (lmhi * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         lmlo = (lmlo * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_UP]) >>
            PNG_COST_SHIFT;
         lmhi = (lmhi * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_UP]) >>
            PNG_COST_SHIFT;
         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;
         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif
        for (i = 0, rp = row_buf + 1, dp = png_ptr->up_row + 1,
             pp = prev_row + 1; i < row_bytes; i++)
        {
          v = *dp++ = (png_byte)(((int)*rp++ - (int)*pp++) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
 
          if (sum > lmins)   
              break;
        }
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_UP)
            {
               sumlo = (sumlo * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               sumhi = (sumhi * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         sumlo = (sumlo * png_ptr->filter_costs[PNG_FILTER_VALUE_UP]) >>
            PNG_COST_SHIFT;
         sumhi = (sumhi * png_ptr->filter_costs[PNG_FILTER_VALUE_UP]) >>
            PNG_COST_SHIFT;
         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;
         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
        if (sum < mins)
        {
           mins = sum;
          best_row = png_ptr->up_row;
       }
    }
 
     
    if (filter_to_do == PNG_FILTER_AVG)
    {
       png_bytep rp, dp, pp, lp;
       png_uint_32 i;
       for (i = 0, rp = row_buf + 1, dp = png_ptr->avg_row + 1,
            pp = prev_row + 1; i < bpp; i++)
       {
          *dp++ = (png_byte)(((int)*rp++ - ((int)*pp++ / 2)) & 0xff);
       }
       for (lp = row_buf + 1; i < row_bytes; i++)
       {
          *dp++ = (png_byte)(((int)*rp++ - (((int)*pp++ + (int)*lp++) / 2))
                  & 0xff);
       }
       best_row = png_ptr->avg_row;
    }
 
    else if (filter_to_do & PNG_FILTER_AVG)
    {
       png_bytep rp, dp, pp, lp;
       png_uint_32 sum = 0, lmins = mins;
        png_uint_32 i;
        int v;
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_AVG)
            {
               lmlo = (lmlo * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               lmhi = (lmhi * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         lmlo = (lmlo * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_AVG]) >>
            PNG_COST_SHIFT;
         lmhi = (lmhi * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_AVG]) >>
            PNG_COST_SHIFT;
         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;
         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif
        for (i = 0, rp = row_buf + 1, dp = png_ptr->avg_row + 1,
             pp = prev_row + 1; i < bpp; i++)
        {
          v = *dp++ = (png_byte)(((int)*rp++ - ((int)*pp++ / 2)) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
       }
       for (lp = row_buf + 1; i < row_bytes; i++)
       {
          v = *dp++ =
           (png_byte)(((int)*rp++ - (((int)*pp++ + (int)*lp++) / 2)) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
 
          if (sum > lmins)   
              break;
        }
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_NONE)
            {
               sumlo = (sumlo * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               sumhi = (sumhi * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         sumlo = (sumlo * png_ptr->filter_costs[PNG_FILTER_VALUE_AVG]) >>
            PNG_COST_SHIFT;
         sumhi = (sumhi * png_ptr->filter_costs[PNG_FILTER_VALUE_AVG]) >>
            PNG_COST_SHIFT;
         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;
         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
        if (sum < mins)
        {
           mins = sum;
          best_row = png_ptr->avg_row;
       }
    }
 
     
    if (filter_to_do == PNG_FILTER_PAETH)
    {
       png_bytep rp, dp, pp, cp, lp;
       png_uint_32 i;
       for (i = 0, rp = row_buf + 1, dp = png_ptr->paeth_row + 1,
            pp = prev_row + 1; i < bpp; i++)
       {
          *dp++ = (png_byte)(((int)*rp++ - (int)*pp++) & 0xff);
       }
 
       for (lp = row_buf + 1, cp = prev_row + 1; i < row_bytes; i++)
       {
          int a, b, c, pa, pb, pc, p;
 
          b = *pp++;
          c = *cp++;
          a = *lp++;
 
          p = b - c;
          pc = a - c;
 
 #ifdef PNG_USE_ABS
          pa = abs(p);
          pb = abs(pc);
          pc = abs(p + pc);
 #else
          pa = p < 0 ? -p : p;
          pb = pc < 0 ? -pc : pc;
          pc = (p + pc) < 0 ? -(p + pc) : p + pc;
 #endif
 
          p = (pa <= pb && pa <=pc) ? a : (pb <= pc) ? b : c;
 
          *dp++ = (png_byte)(((int)*rp++ - p) & 0xff);
       }
       best_row = png_ptr->paeth_row;
    }
 
    else if (filter_to_do & PNG_FILTER_PAETH)
    {
       png_bytep rp, dp, pp, cp, lp;
       png_uint_32 sum = 0, lmins = mins;
        png_uint_32 i;
        int v;
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 lmhi, lmlo;
         lmlo = lmins & PNG_LOMASK;
         lmhi = (lmins >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_PAETH)
            {
               lmlo = (lmlo * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               lmhi = (lmhi * png_ptr->inv_filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         lmlo = (lmlo * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_PAETH]) >>
            PNG_COST_SHIFT;
         lmhi = (lmhi * png_ptr->inv_filter_costs[PNG_FILTER_VALUE_PAETH]) >>
            PNG_COST_SHIFT;
         if (lmhi > PNG_HIMASK)
            lmins = PNG_MAXSUM;
         else
            lmins = (lmhi << PNG_HISHIFT) + lmlo;
      }
#endif
        for (i = 0, rp = row_buf + 1, dp = png_ptr->paeth_row + 1,
             pp = prev_row + 1; i < bpp; i++)
        {
          v = *dp++ = (png_byte)(((int)*rp++ - (int)*pp++) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
       }
 
       for (lp = row_buf + 1, cp = prev_row + 1; i < row_bytes; i++)
       {
          int a, b, c, pa, pb, pc, p;
 
          b = *pp++;
          c = *cp++;
          a = *lp++;
 
 #ifndef PNG_SLOW_PAETH
          p = b - c;
          pc = a - c;
 #ifdef PNG_USE_ABS
          pa = abs(p);
          pb = abs(pc);
          pc = abs(p + pc);
 #else
          pa = p < 0 ? -p : p;
          pb = pc < 0 ? -pc : pc;
          pc = (p + pc) < 0 ? -(p + pc) : p + pc;
 #endif
          p = (pa <= pb && pa <=pc) ? a : (pb <= pc) ? b : c;
 #else  
          p = a + b - c;
          pa = abs(p - a);
          pb = abs(p - b);
          pc = abs(p - c);
          if (pa <= pb && pa <= pc)
             p = a;
          else if (pb <= pc)
             p = b;
          else
             p = c;
 #endif  
 
          v = *dp++ = (png_byte)(((int)*rp++ - p) & 0xff);
 
          sum += (v < 128) ? v : 256 - v;
 
          if (sum > lmins)   
              break;
        }
  
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
      if (png_ptr->heuristic_method == PNG_FILTER_HEURISTIC_WEIGHTED)
      {
         int j;
         png_uint_32 sumhi, sumlo;
         sumlo = sum & PNG_LOMASK;
         sumhi = (sum >> PNG_HISHIFT) & PNG_HIMASK;
         for (j = 0; j < num_p_filters; j++)
         {
            if (png_ptr->prev_filters[j] == PNG_FILTER_VALUE_PAETH)
            {
               sumlo = (sumlo * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
               sumhi = (sumhi * png_ptr->filter_weights[j]) >>
                  PNG_WEIGHT_SHIFT;
            }
         }
         sumlo = (sumlo * png_ptr->filter_costs[PNG_FILTER_VALUE_PAETH]) >>
            PNG_COST_SHIFT;
         sumhi = (sumhi * png_ptr->filter_costs[PNG_FILTER_VALUE_PAETH]) >>
            PNG_COST_SHIFT;
         if (sumhi > PNG_HIMASK)
            sum = PNG_MAXSUM;
         else
            sum = (sumhi << PNG_HISHIFT) + sumlo;
      }
#endif
        if (sum < mins)
        {
           best_row = png_ptr->paeth_row;
       }
    }
 #endif  
      
  
     png_write_filtered_row(png_ptr, best_row);
#ifdef PNG_WRITE_FILTER_SUPPORTED
#ifdef PNG_WRITE_WEIGHTED_FILTER_SUPPORTED
    
   if (png_ptr->num_prev_filters > 0)
   {
      int j;
      for (j = 1; j < num_p_filters; j++)
      {
         png_ptr->prev_filters[j] = png_ptr->prev_filters[j - 1];
      }
      png_ptr->prev_filters[j] = best_row[0];
   }
#endif
#endif  
  }
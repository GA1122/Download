   psh_glyph_find_strong_points( PSH_Glyph  glyph,
                                 FT_Int     dimension )
   {
      
      
 
     PSH_Hint_Table  table     = &glyph->hint_tables[dimension];
     PS_Mask         mask      = table->hint_masks->masks;
     FT_UInt         num_masks = table->hint_masks->num_masks;
     FT_UInt         first     = 0;
     FT_Int          major_dir = dimension == 0 ? PSH_DIR_VERTICAL
                                                : PSH_DIR_HORIZONTAL;
     PSH_Dimension   dim       = &glyph->globals->dimension[dimension];
     FT_Fixed        scale     = dim->scale_mult;
     FT_Int          threshold;
 
 
     threshold = (FT_Int)FT_DivFix( PSH_STRONG_THRESHOLD, scale );
     if ( threshold > PSH_STRONG_THRESHOLD_MAXIMUM )
       threshold = PSH_STRONG_THRESHOLD_MAXIMUM;
 
      
       
      if ( num_masks > 1 && glyph->num_points > 0 )
      {
      first = mask->end_point;
//        
//       first = mask->end_point > glyph->num_points
//                 ? glyph->num_points
//                 : mask->end_point;
        mask++;
        for ( ; num_masks > 1; num_masks--, mask++ )
        {
         next  = mask->end_point;
          FT_Int   count;
  
  
        next  = mask->end_point;
//         next  = mask->end_point > glyph->num_points
//                   ? glyph->num_points
//                   : mask->end_point;
          count = next - first;
          if ( count > 0 )
          {
                                              threshold, major_dir );
         }
         first = next;
       }
     }
 
      
     if ( num_masks == 1 )
     {
       FT_UInt    count = glyph->num_points;
       PSH_Point  point = glyph->points;
 
 
       psh_hint_table_activate_mask( table, table->hint_masks->masks );
 
       psh_hint_table_find_strong_points( table, point, count,
                                          threshold, major_dir );
     }
 
      
      
     {
       FT_UInt    count = glyph->num_points;
       PSH_Point  point = glyph->points;
 
 
       for ( ; count > 0; count--, point++ )
         if ( point->hint && !psh_point_is_strong( point ) )
           psh_point_set_strong( point );
     }
   }
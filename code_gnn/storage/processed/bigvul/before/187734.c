  transform_display_init(transform_display *dp, png_modifier *pm, png_uint_32 id,
    PNG_CONST image_transform *transform_list)
//     const image_transform *transform_list)
  {
     memset(dp, 0, sizeof *dp);
  
      
   standard_display_init(&dp->this, &pm->this, id, 0 ,
//    standard_display_init(&dp->this, &pm->this, id, do_read_interlace,
        pm->use_update_info);
  
      
     dp->pm = pm;
     dp->transform_list = transform_list;
//    dp->max_gamma_8 = 16;
  
      
     dp->output_colour_type = 255;  
     dp->output_bit_depth = 255;   
//    dp->unpacked = 0;  
  }
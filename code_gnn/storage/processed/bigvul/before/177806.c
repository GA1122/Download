 int seticc(i_ctx_t * i_ctx_p, int ncomps, ref *ICCdict, float *range_buff)
 {
     int                     code, k;
     gs_color_space *        pcs;
     ref *                   pstrmval;
     stream *                s = 0L;
     cmm_profile_t           *picc_profile = NULL;
     int                     i, expected = 0;
     ref *                   pnameval;
     static const char *const icc_std_profile_names[] = {
             GSICC_STANDARD_PROFILES
         };
     static const char *const icc_std_profile_keys[] = {
             GSICC_STANDARD_PROFILES_KEYS
         };
 
      
     if (dict_find_string(ICCdict, "DataSource", &pstrmval) <= 0)
         return_error(gs_error_undefined);
     check_read_file(i_ctx_p, s, pstrmval);
 
      
     code = gs_cspace_build_ICC(&pcs, NULL, gs_gstate_memory(igs));
     if (code < 0)
         return gs_rethrow(code, "building color space object");
      
       
    if (dict_find_string(ICCdict, "Name", &pnameval) > 0){
//     if (dict_find_string(ICCdict, "Name", &pnameval) > 0 && r_has_type(pnameval, t_string)){
          uint size = r_size(pnameval);
          char *str = (char *)gs_alloc_bytes(gs_gstate_memory(igs), size+1, "seticc");
          memcpy(str, (const char *)pnameval->value.bytes, size);
         str[size] = 0;
 
          
         for (k = 0; k < GSICC_NUMBER_STANDARD_PROFILES; k++) {
             if ( strcmp( str, icc_std_profile_keys[k] ) == 0 ) {
                 picc_profile = gsicc_get_profile_handle_file(icc_std_profile_names[k],
                     strlen(icc_std_profile_names[k]), gs_gstate_memory(igs));
                 break;
             }
         }
         gs_free_object(gs_gstate_memory(igs), str, "seticc");
     } else {
         picc_profile = gsicc_profile_new(s, gs_gstate_memory(igs), NULL, 0);
         if (picc_profile == NULL)
             return gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
          
         picc_profile->profile_handle =
             gsicc_get_profile_handle_buffer(picc_profile->buffer,
                                             picc_profile->buffer_size,
                                             gs_gstate_memory(igs));
     }
     if (picc_profile == NULL || picc_profile->profile_handle == NULL) {
          
         rc_decrement(picc_profile,"seticc");
         rc_decrement(pcs,"seticc");
         return -1;
     }
     code = gsicc_set_gscs_profile(pcs, picc_profile, gs_gstate_memory(igs));
     if (code < 0) {
         rc_decrement(picc_profile,"seticc");
         rc_decrement(pcs,"seticc");
         return code;
     }
     picc_profile->num_comps = ncomps;
 
     picc_profile->data_cs =
         gscms_get_profile_data_space(picc_profile->profile_handle,
             picc_profile->memory);
     switch (picc_profile->data_cs) {
         case gsCIEXYZ:
         case gsCIELAB:
         case gsRGB:
             expected = 3;
             break;
         case gsGRAY:
             expected = 1;
             break;
         case gsCMYK:
             expected = 4;
             break;
         case gsNCHANNEL:
         case gsNAMED:             
         case gsUNDEFINED:         
             break;
     }
     if (!expected || ncomps != expected) {
         rc_decrement(picc_profile,"seticc");
         rc_decrement(pcs,"seticc");
         return_error(gs_error_rangecheck);
     }
 
      
      
     gsicc_init_hash_cs(picc_profile, igs);
 
      
     if (picc_profile->data_cs == gsCIELAB) {
      
          
         picc_profile->Range.ranges[0].rmin = 0.0;
         picc_profile->Range.ranges[0].rmax = 100.0;
         picc_profile->Range.ranges[1].rmin = -128.0;
         picc_profile->Range.ranges[1].rmax = 127.0;
         picc_profile->Range.ranges[2].rmin = -128.0;
         picc_profile->Range.ranges[2].rmax = 127.0;
         picc_profile->islab = true;
     } else {
         for (i = 0; i < ncomps; i++) {
             picc_profile->Range.ranges[i].rmin = range_buff[2 * i];
             picc_profile->Range.ranges[i].rmax = range_buff[2 * i + 1];
         }
     }
      
     if (gs_currentoverrideicc(igs) && picc_profile->data_cs != gsCIELAB) {
          
         switch( picc_profile->data_cs ) {
             case gsRGB:
                 pcs->cmm_icc_profile_data = igs->icc_manager->default_rgb;
                 break;
             case gsGRAY:
                 pcs->cmm_icc_profile_data = igs->icc_manager->default_gray;
                 break;
             case gsCMYK:
                 pcs->cmm_icc_profile_data = igs->icc_manager->default_cmyk;
                 break;
             default:
                 break;
         }
          
         rc_adjust(picc_profile, -2, "seticc");
         rc_increment(pcs->cmm_icc_profile_data);
     }
      
     code = gs_setcolorspace(igs, pcs);
      
     rc_decrement_only(pcs, "seticc");
      
     rc_decrement(picc_profile,"seticc");
      
     pop(1);
     return code;
 }
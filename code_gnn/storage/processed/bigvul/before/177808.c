 zset_outputintent(i_ctx_t * i_ctx_p)
 {
     os_ptr                  op = osp;
     int                     code = 0;
     gx_device *dev = gs_currentdevice(igs);
     cmm_dev_profile_t       *dev_profile;
     stream *                s = 0L;
     ref *                   pnval;
     ref *                   pstrmval;
     int                     ncomps, dev_comps;
     cmm_profile_t           *picc_profile;
     int                     expected = 0;
     gs_color_space_index    index;
     gsicc_manager_t         *icc_manager = igs->icc_manager;
     cmm_profile_t           *source_profile = NULL;
 
     check_type(*op, t_dictionary);
     check_dict_read(*op);
     if_debug0m(gs_debug_flag_icc, imemory, "[icc] Using OutputIntent\n");
 
      
     code = dev_proc(dev, get_profile)(dev,  &dev_profile);
     if (code < 0)
         return code;
 
     if (dev_profile == NULL) {
         code = gsicc_init_device_profile_struct(dev, NULL, 0);
         if (code < 0)
             return code;
         code = dev_proc(dev, get_profile)(dev,  &dev_profile);
         if (code < 0)
             return code;
     }
     if (dev_profile->oi_profile != NULL) {
         return 0;   
     }
     code = dict_find_string(op, "N", &pnval);
     if (code < 0)
          return code;
      if (code == 0)
          return_error(gs_error_undefined);
//     if (r_type(pnval) != t_integer)
//         return gs_note_error(gs_error_typecheck);
      ncomps = pnval->value.intval;
  
       
     check_read_file(i_ctx_p, s, pstrmval);
 
     picc_profile = gsicc_profile_new(s, gs_gstate_memory(igs), NULL, 0);
     if (picc_profile == NULL)
         return gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
     picc_profile->num_comps = ncomps;
     picc_profile->profile_handle =
         gsicc_get_profile_handle_buffer(picc_profile->buffer,
                                         picc_profile->buffer_size,
                                         gs_gstate_memory(igs));
     if (picc_profile->profile_handle == NULL) {
         rc_decrement(picc_profile,"zset_outputintent");
         return -1;
     }
     picc_profile->data_cs =
         gscms_get_profile_data_space(picc_profile->profile_handle,
             picc_profile->memory);
     switch (picc_profile->data_cs) {
         case gsCIEXYZ:
         case gsCIELAB:
         case gsRGB:
             expected = 3;
             source_profile = icc_manager->default_rgb;
             break;
         case gsGRAY:
             expected = 1;
             source_profile = icc_manager->default_gray;
             break;
         case gsCMYK:
             expected = 4;
             source_profile = icc_manager->default_cmyk;
             break;
         case gsNCHANNEL:
             expected = 0;
             break;
         case gsNAMED:
         case gsUNDEFINED:
             break;
     }
     if (expected && ncomps != expected) {
         rc_decrement(picc_profile,"zset_outputintent");
         return_error(gs_error_rangecheck);
     }
     gsicc_init_hash_cs(picc_profile, igs);
 
      
     dev_profile->oi_profile = picc_profile;
     picc_profile->name = (char *) gs_alloc_bytes(picc_profile->memory,
                                                  MAX_DEFAULT_ICC_LENGTH,
                                                  "zset_outputintent");
     strncpy(picc_profile->name, OI_PROFILE, strlen(OI_PROFILE));
     picc_profile->name[strlen(OI_PROFILE)] = 0;
     picc_profile->name_length = strlen(OI_PROFILE);
      
     gsicc_set_icc_range(&picc_profile);
 
      
 
     dev_comps = dev_profile->device_profile[0]->num_comps;
     index = gsicc_get_default_type(dev_profile->device_profile[0]);
     if (ncomps == dev_comps && index < gs_color_space_index_DevicePixel) {
          
         rc_assign(dev_profile->device_profile[0], picc_profile,
                   "zset_outputintent");
         if_debug0m(gs_debug_flag_icc, imemory, "[icc] OutputIntent used for device profile\n");
     } else {
         if (dev_profile->proof_profile == NULL) {
              
             dev_profile->proof_profile = picc_profile;
             rc_increment(picc_profile);
             if_debug0m(gs_debug_flag_icc, imemory, "[icc] OutputIntent used for proof profile\n");
         }
     }
      
     index = gsicc_get_default_type(source_profile);
     if (index < gs_color_space_index_DevicePixel) {
          
         switch (picc_profile->data_cs) {
             case gsGRAY:
                 if_debug0m(gs_debug_flag_icc, imemory, "[icc] OutputIntent used source Gray\n");
                 rc_assign(icc_manager->default_gray, picc_profile,
                           "zset_outputintent");
                 break;
             case gsRGB:
                 if_debug0m(gs_debug_flag_icc, imemory, "[icc] OutputIntent used source RGB\n");
                 rc_assign(icc_manager->default_rgb, picc_profile,
                           "zset_outputintent");
                 break;
             case gsCMYK:
                 if_debug0m(gs_debug_flag_icc, imemory, "[icc] OutputIntent used source CMYK\n");
                 rc_assign(icc_manager->default_cmyk, picc_profile,
                           "zset_outputintent");
                 break;
             default:
                 break;
         }
     }
      
     pop(1);
     return code;
 }
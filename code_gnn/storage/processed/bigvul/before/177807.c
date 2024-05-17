 znumicc_components(i_ctx_t * i_ctx_p)
 {
     ref *                   pnval;
     ref *                   pstrmval;
     stream *                s;
     int                     ncomps, expected = 0, code;
     cmm_profile_t           *picc_profile;
     os_ptr                  op = osp;
 
     check_type(*op, t_dictionary);
     check_dict_read(*op);
 
     code = dict_find_string(op, "N", &pnval);
     if (code < 0)
         return code;
     if (code == 0)
          return code;
      if (code == 0)
          return_error(gs_error_undefined);
//     if (r_type(pnval) != t_integer)
//         return gs_note_error(gs_error_typecheck);
      ncomps = pnval->value.intval;
       
      if (dict_find_string(op, "DataSource", &pstrmval) <= 0)
     if (picc_profile == NULL)
         return gs_throw(gs_error_VMerror, "Creation of ICC profile failed");
 
     picc_profile->num_comps = ncomps;
     picc_profile->profile_handle =
         gsicc_get_profile_handle_buffer(picc_profile->buffer,
                                         picc_profile->buffer_size,
                                         gs_gstate_memory(igs));
     if (picc_profile->profile_handle == NULL) {
         rc_decrement(picc_profile,"znumicc_components");
         make_int(op, expected);
         return 0;
     }
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
             expected = 0;
             break;
         case gsNAMED:
         case gsUNDEFINED:
             expected = -1;
             break;
     }
 
     make_int(op, expected);
 
     rc_decrement(picc_profile,"zset_outputintent");
     return 0;
 }
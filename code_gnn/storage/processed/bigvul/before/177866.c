 gs_call_interp(i_ctx_t **pi_ctx_p, ref * pref, int user_errors,
                int *pexit_code, ref * perror_object)
 {
     ref *epref = pref;
     ref doref;
     ref *perrordict;
     ref error_name;
     int code, ccode;
     ref saref;
     i_ctx_t *i_ctx_p = *pi_ctx_p;
     int *gc_signal = &imemory_system->gs_lib_ctx->gcsignal;
 
     *pexit_code = 0;
     *gc_signal = 0;
     ialloc_reset_requested(idmemory);
 again:
      
     make_null(perror_object);
     o_stack.requested = e_stack.requested = d_stack.requested = 0;
     while (*gc_signal) {  
         gs_gc_root_t epref_root;
 
         *gc_signal = 0;
          
          
         gs_register_ref_root(imemory_system, &epref_root,
                              (void **)&epref, "gs_call_interp(epref)");
         code = interp_reclaim(pi_ctx_p, -1);
         i_ctx_p = *pi_ctx_p;
         gs_unregister_root(imemory_system, &epref_root,
                            "gs_call_interp(epref)");
         if (code < 0)
             return code;
     }
     code = interp(pi_ctx_p, epref, perror_object);
     i_ctx_p = *pi_ctx_p;
     if (!r_has_type(&i_ctx_p->error_object, t__invalid)) {
         *perror_object = i_ctx_p->error_object;
         make_t(&i_ctx_p->error_object, t__invalid);
     }
      
      
      
     *gc_signal = 0;
     set_gc_signal(i_ctx_p, 1);
     if (esp < esbot)             
         esp = esbot;
     switch (code) {
         case gs_error_Fatal:
             *pexit_code = 255;
             return code;
         case gs_error_Quit:
             *perror_object = osp[-1];
             *pexit_code = code = osp->value.intval;
             osp -= 2;
             return
                 (code == 0 ? gs_error_Quit :
                  code < 0 && code > -100 ? code : gs_error_Fatal);
         case gs_error_InterpreterExit:
             return 0;
         case gs_error_ExecStackUnderflow:
  
             ref_stack_pop_block(&e_stack);
             doref = *perror_object;
             epref = &doref;
             goto again;
         case gs_error_VMreclaim:
              
              
             (void)interp_reclaim(pi_ctx_p,
                                   (osp->value.intval == 2 ?
                                    avm_global : avm_local));
             i_ctx_p = *pi_ctx_p;
             make_oper(&doref, 0, zpop);
             epref = &doref;
             goto again;
         case gs_error_NeedInput:
         case gs_error_interrupt:
             return code;
     }
      
     if (osp < osbot - 1)
         osp = osbot - 1;
      
      
     switch (code) {
         case gs_error_dictstackoverflow:
              
              
              
              
             if (osp >= ostop) {
                 if ((ccode = ref_stack_extend(&o_stack, 1)) < 0)
                     return ccode;
             }
              
             ccode = copy_stack(i_ctx_p, &d_stack, min_dstack_size, &saref);
             if (ccode < 0)
                 return ccode;
             ref_stack_pop_to(&d_stack, min_dstack_size);
             dict_set_top();
             *++osp = saref;
             break;
         case gs_error_dictstackunderflow:
             if (ref_stack_pop_block(&d_stack) >= 0) {
                 dict_set_top();
                 doref = *perror_object;
                 epref = &doref;
                 goto again;
             }
             break;
         case gs_error_execstackoverflow:
              
              
              
              
             if (osp >= ostop) {
                 if ((ccode = ref_stack_extend(&o_stack, 1)) < 0)
                     return ccode;
             }
             ccode = copy_stack(i_ctx_p, &e_stack, 0, &saref);
             if (ccode < 0)
                 return ccode;
             {
                 uint count = ref_stack_count(&e_stack);
                 uint limit = ref_stack_max_count(&e_stack) - ES_HEADROOM;
 
                 if (count > limit) {
                      
                     int skip = count - limit;
                     int i;
 
                     for (i = skip; i < skip + MIN_BLOCK_ESTACK; ++i) {
                         const ref *ep = ref_stack_index(&e_stack, i);
 
                         if (r_has_type_attrs(ep, t_null, a_executable)) {
                             skip = i + 1;
                             break;
                         }
                     }
                     pop_estack(i_ctx_p, skip);
                 }
             }
             *++osp = saref;
             break;
         case gs_error_stackoverflow:
             if (ref_stack_extend(&o_stack, o_stack.requested) >= 0) {    
                  
                  
                 doref = *perror_object;
                 if (r_is_proc(&doref)) {
                     *++osp = doref;
                     make_null_proc(&doref);
                 }
                 epref = &doref;
                 goto again;
             }
             ccode = copy_stack(i_ctx_p, &o_stack, 0, &saref);
             if (ccode < 0)
                 return ccode;
             ref_stack_clear(&o_stack);
             *++osp = saref;
             break;
         case gs_error_stackunderflow:
             if (ref_stack_pop_block(&o_stack) >= 0) {
                 doref = *perror_object;
                 epref = &doref;
                 goto again;
             }
             break;
     }
     if (user_errors < 0)
         return code;
     if (gs_errorname(i_ctx_p, code, &error_name) < 0)
         return code;             
      
     if (dict_find_string(systemdict, "errordict", &perrordict) <= 0 ||
         (dict_find(perrordict, &error_name, &epref) <= 0 &&
          (dict_find_string(systemdict, "gserrordict", &perrordict) <= 0 ||
           dict_find(perrordict, &error_name, &epref) <= 0))
         )
         return code;             
     doref = *epref;
     epref = &doref;
       
      if (!GS_ERROR_IS_INTERRUPT(code)) {
           
        *++osp = *perror_object;
//         osp++;
//         if (osp >= ostop) {
//             *pexit_code = gs_error_Fatal;
//             return_error(gs_error_Fatal);
//         }
//         *osp = *perror_object;
          errorexec_find(i_ctx_p, osp);
      }
      goto again;
 }
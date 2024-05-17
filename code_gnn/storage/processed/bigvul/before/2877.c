interp(i_ctx_t **pi_ctx_p  ,
       const ref * pref  ,
       ref * perror_object)
{
    i_ctx_t *i_ctx_p = *pi_ctx_p;
     
    register const ref_packed *iref_packed = (const ref_packed *)pref;
     
#ifdef ALIGNMENT_ALIASING_BUG
    const ref *iref_temp;
#  define IREF (iref_temp = (const ref *)iref_packed, iref_temp)
#else
#  define IREF ((const ref *)iref_packed)
#endif
#define SET_IREF(rp) (iref_packed = (const ref_packed *)(rp))
    register int icount = 0;     
    register os_ptr iosp = osp;  
    register es_ptr iesp = esp;  
    int code;
    ref token;                   
                                 
    ref *pvalue;
    ref refnull;
    uint opindex;                
    os_ptr whichp;

     
    struct interp_error_s {
        int code;
        int line;
        const ref *obj;
        ref full;
    } ierror;

     
    const name_table *const int_nt = imemory->gs_lib_ctx->gs_name_table;

#define set_error(ecode)\
  { ierror.code = ecode; ierror.line = __LINE__; }
#define return_with_error(ecode, objp)\
  { set_error(ecode); ierror.obj = objp; goto rwe; }
#define return_with_error_iref(ecode)\
  { set_error(ecode); goto rwei; }
#define return_with_code_iref()\
  { ierror.line = __LINE__; goto rweci; }
#define return_with_stackoverflow(objp)\
  { o_stack.requested = 1; return_with_error(gs_error_stackoverflow, objp); }
#define return_with_stackoverflow_iref()\
  { o_stack.requested = 1; return_with_error_iref(gs_error_stackoverflow); }
 
#define return_with_error_tx_op(err_code)\
  { if (r_has_type(IREF, t_name)) {\
        return_with_error(err_code, pvalue);\
    } else {\
        return_with_error_iref(err_code);\
    }\
  }

    int *ticks_left = &imemory_system->gs_lib_ctx->gcsignal;

#if defined(DEBUG_TRACE_PS_OPERATORS) || defined(DEBUG)
    int (*call_operator_fn)(op_proc_t, i_ctx_t *) = do_call_operator;

    if (gs_debug_c('!'))
        call_operator_fn = do_call_operator_verbose;
#endif

    *ticks_left = i_ctx_p->time_slice_ticks;

     make_null(&ierror.full);
     ierror.obj = &ierror.full;
     make_null(&refnull);
     pvalue = &refnull;

     
    set_gc_signal(i_ctx_p, -100);

    esfile_clear_cache();
     
#define IREF_NEXT(ip)\
  ((const ref_packed *)((const ref *)(ip) + 1))
#define IREF_NEXT_EITHER(ip)\
  ( r_is_packed(ip) ? (ip) + 1 : IREF_NEXT(ip) )
#define store_state(ep)\
  ( icount > 0 ? (ep->value.const_refs = IREF + 1, r_set_size(ep, icount)) : 0 )
#define store_state_short(ep)\
  ( icount > 0 ? (ep->value.packed = iref_packed + 1, r_set_size(ep, icount)) : 0 )
#define store_state_either(ep)\
  ( icount > 0 ? (ep->value.packed = IREF_NEXT_EITHER(iref_packed), r_set_size(ep, icount)) : 0 )
#define next()\
  if ( --icount > 0 ) { iref_packed = IREF_NEXT(iref_packed); goto top; } else goto out
#define next_short()\
  if ( --icount <= 0 ) { if ( icount < 0 ) goto up; iesp--; }\
  ++iref_packed; goto top
#define next_either()\
  if ( --icount <= 0 ) { if ( icount < 0 ) goto up; iesp--; }\
  iref_packed = IREF_NEXT_EITHER(iref_packed); goto top

#if !PACKED_SPECIAL_OPS
#  undef next_either
#  define next_either() next()
#  undef store_state_either
#  define store_state_either(ep) store_state(ep)
#endif

     
     
     
    if (iesp >= estop)
        return_with_error(gs_error_execstackoverflow, pref);
    ++iesp;
    ref_assign_inline(iesp, pref);
    goto bot;
  top:
         
    INCR(top);
#ifdef DEBUG
     
    if (iosp >= osbot &&
        (r_type(iosp) == t__invalid || r_type(iosp) >= tx_next_op)
        ) {
        mlprintf(imemory, "Invalid value on o-stack!\n");
        return_with_error_iref(gs_error_Fatal);
    }
    if (gs_debug['I'] ||
        (gs_debug['i'] &&
         (r_is_packed(iref_packed) ?
          r_packed_is_name(iref_packed) :
          r_has_type(IREF, t_name)))
        ) {
        os_ptr save_osp = osp;   
        es_ptr save_esp = esp;

        osp = iosp;
        esp = iesp;
        dmlprintf5(imemory, "d%u,e%u<%u>0x%lx(%d): ",
                  ref_stack_count(&d_stack), ref_stack_count(&e_stack),
                  ref_stack_count(&o_stack), (ulong)IREF, icount);
        debug_print_ref(imemory, IREF);
        if (iosp >= osbot) {
            dmputs(imemory, " // ");
            debug_print_ref(imemory, iosp);
        }
        dmputc(imemory, '\n');
        osp = save_osp;
        esp = save_esp;
        dmflush(imemory);
    }
#endif
 
 
#define lit(t) type_xe_value(t, a_execute)
#define exec(t) type_xe_value(t, a_execute + a_executable)
#define nox(t) type_xe_value(t, 0)
#define nox_exec(t) type_xe_value(t, a_executable)
#define plain(t) type_xe_value(t, 0)
#define plain_exec(t) type_xe_value(t, a_executable)
     
    switch (r_type_xe(iref_packed)) {
             
#define cases_invalid()\
  case plain(t__invalid): case plain_exec(t__invalid)
          cases_invalid():
            return_with_error_iref(gs_error_Fatal);
#define cases_nox()\
  case nox_exec(t_array): case nox_exec(t_dictionary):\
  case nox_exec(t_file): case nox_exec(t_string):\
  case nox_exec(t_mixedarray): case nox_exec(t_shortarray)
          cases_nox():
            return_with_error_iref(gs_error_invalidaccess);
             
#define cases_lit_1()\
  case lit(t_array): case nox(t_array):\
  case plain(t_boolean): case plain_exec(t_boolean):\
  case lit(t_dictionary): case nox(t_dictionary)
#define cases_lit_2()\
  case lit(t_file): case nox(t_file):\
  case plain(t_fontID): case plain_exec(t_fontID):\
  case plain(t_integer): case plain_exec(t_integer):\
  case plain(t_mark): case plain_exec(t_mark)
#define cases_lit_3()\
  case plain(t_name):\
  case plain(t_null):\
  case plain(t_oparray):\
  case plain(t_operator)
#define cases_lit_4()\
  case plain(t_real): case plain_exec(t_real):\
  case plain(t_save): case plain_exec(t_save):\
  case lit(t_string): case nox(t_string)
#define cases_lit_5()\
  case lit(t_mixedarray): case nox(t_mixedarray):\
  case lit(t_shortarray): case nox(t_shortarray):\
  case plain(t_device): case plain_exec(t_device):\
  case plain(t_struct): case plain_exec(t_struct):\
  case plain(t_astruct): case plain_exec(t_astruct)
             
#define cases_lit_array()\
  case exec(t_array): case exec(t_mixedarray): case exec(t_shortarray)
          cases_lit_1():
          cases_lit_2():
          cases_lit_3():
          cases_lit_4():
          cases_lit_5():
            INCR(lit);
            break;
          cases_lit_array():
            INCR(lit_array);
            break;
             
        case plain_exec(tx_op_add):
x_add:      INCR(x_add);
            osp = iosp;  
            if ((code = zop_add(i_ctx_p)) < 0)
                return_with_error_tx_op(code);
            iosp--;
            next_either();
        case plain_exec(tx_op_def):
x_def:      INCR(x_def);
            osp = iosp;  
            if ((code = zop_def(i_ctx_p)) < 0)
                return_with_error_tx_op(code);
            iosp -= 2;
            next_either();
        case plain_exec(tx_op_dup):
x_dup:      INCR(x_dup);
            if (iosp < osbot)
                return_with_error_tx_op(gs_error_stackunderflow);
            if (iosp >= ostop) {
                o_stack.requested = 1;
                return_with_error_tx_op(gs_error_stackoverflow);
            }
            iosp++;
            ref_assign_inline(iosp, iosp - 1);
            next_either();
        case plain_exec(tx_op_exch):
x_exch:     INCR(x_exch);
            if (iosp <= osbot)
                return_with_error_tx_op(gs_error_stackunderflow);
            ref_assign_inline(&token, iosp);
            ref_assign_inline(iosp, iosp - 1);
            ref_assign_inline(iosp - 1, &token);
            next_either();
        case plain_exec(tx_op_if):
x_if:       INCR(x_if);
            if (!r_is_proc(iosp))
                return_with_error_tx_op(check_proc_failed(iosp));
            if (!r_has_type(iosp - 1, t_boolean))
                return_with_error_tx_op((iosp <= osbot ?
                                        gs_error_stackunderflow : gs_error_typecheck));
            if (!iosp[-1].value.boolval) {
                iosp -= 2;
                next_either();
            }
            if (iesp >= estop)
                return_with_error_tx_op(gs_error_execstackoverflow);
            store_state_either(iesp);
            whichp = iosp;
            iosp -= 2;
            goto ifup;
        case plain_exec(tx_op_ifelse):
x_ifelse:   INCR(x_ifelse);
            if (!r_is_proc(iosp))
                return_with_error_tx_op(check_proc_failed(iosp));
            if (!r_is_proc(iosp - 1))
                return_with_error_tx_op(check_proc_failed(iosp - 1));
            if (!r_has_type(iosp - 2, t_boolean))
                return_with_error_tx_op((iosp < osbot + 2 ?
                                        gs_error_stackunderflow : gs_error_typecheck));
            if (iesp >= estop)
                return_with_error_tx_op(gs_error_execstackoverflow);
            store_state_either(iesp);
            whichp = (iosp[-2].value.boolval ? iosp - 1 : iosp);
            iosp -= 3;
             
          ifup:if ((icount = r_size(whichp) - 1) <= 0) {
                if (icount < 0)
                    goto up;     
                SET_IREF(whichp->value.refs);    
                if (--(*ticks_left) > 0)
                    goto top;
            }
            ++iesp;
             
            iesp->tas = whichp->tas;
            SET_IREF(iesp->value.refs = whichp->value.refs);
            if (--(*ticks_left) > 0)
                goto top;
            goto slice;
        case plain_exec(tx_op_index):
x_index:    INCR(x_index);
            osp = iosp;  
            if ((code = zindex(i_ctx_p)) < 0)
                return_with_error_tx_op(code);
            next_either();
        case plain_exec(tx_op_pop):
x_pop:      INCR(x_pop);
            if (iosp < osbot)
                return_with_error_tx_op(gs_error_stackunderflow);
            iosp--;
            next_either();
        case plain_exec(tx_op_roll):
x_roll:     INCR(x_roll);
            osp = iosp;  
            if ((code = zroll(i_ctx_p)) < 0)
                return_with_error_tx_op(code);
            iosp -= 2;
            next_either();
        case plain_exec(tx_op_sub):
x_sub:      INCR(x_sub);
            osp = iosp;  
            if ((code = zop_sub(i_ctx_p)) < 0)
                return_with_error_tx_op(code);
            iosp--;
            next_either();
             
        case plain_exec(t_null):
            goto bot;
        case plain_exec(t_oparray):
             
            INCR(exec_array);
            opindex = op_index(IREF);
            pvalue = (ref *)IREF->value.const_refs;
          opst:          
            store_state(iesp);
          oppr:          
            if (iesp >= estop - 4)
                return_with_error_iref(gs_error_execstackoverflow);
            iesp += 5;
            osp = iosp;          
            make_mark_estack(iesp - 4, es_other, oparray_cleanup);
            make_int(iesp - 3, opindex);  
            make_int(iesp - 2, ref_stack_count_inline(&o_stack));
            make_int(iesp - 1, ref_stack_count_inline(&d_stack));
            make_op_estack(iesp, oparray_pop);
            goto pr;
          prst:          
            store_state(iesp);
          pr:                    
             
            if (iesp >= estop) {
                return_with_error_iref(gs_error_execstackoverflow);
            }
            if ((icount = r_size(pvalue) - 1) <= 0) {
                if (icount < 0)
                    goto up;     
                SET_IREF(pvalue->value.refs);    
                if (--(*ticks_left) > 0)
                    goto top;
            }
            ++iesp;
             
            iesp->tas = pvalue->tas;
            SET_IREF(iesp->value.refs = pvalue->value.refs);
            if (--(*ticks_left) > 0)
                goto top;
            goto slice;
        case plain_exec(t_operator):
            INCR(exec_operator);
            if (--(*ticks_left) <= 0) {     
                 
 
            }
            esp = iesp;          
            osp = iosp;          
             
             
             
             
             
             
             
             
             
             
             
             
             
             
            switch (code = call_operator(real_opproc(IREF), i_ctx_p)) {
                case 0:  
                case 1:  
                    iosp = osp;
                    next();
                case o_push_estack:      
                    store_state(iesp);
                  opush:iosp = osp;
                    iesp = esp;
                    if (--(*ticks_left) > 0)
                        goto up;
                    goto slice;
                case o_pop_estack:       
                  opop:iosp = osp;
                    if (esp == iesp)
                        goto bot;
                    iesp = esp;
                    goto up;
                case o_reschedule:
                    store_state(iesp);
                    goto res;
                case gs_error_Remap_Color:
oe_remap:           store_state(iesp);
remap:              if (iesp + 2 >= estop) {
                        esp = iesp;
                        code = ref_stack_extend(&e_stack, 2);
                        if (code < 0)
                            return_with_error_iref(code);
                        iesp = esp;
                    }
                    packed_get(imemory, iref_packed, iesp + 1);
                    make_oper(iesp + 2, 0,
                              r_ptr(&istate->remap_color_info,
                                    int_remap_color_info_t)->proc);
                    iesp += 2;
                    goto up;
            }
            iosp = osp;
            iesp = esp;
            return_with_code_iref();
        case plain_exec(t_name):
            INCR(exec_name);
            pvalue = IREF->value.pname->pvalue;
            if (!pv_valid(pvalue)) {
                uint nidx = names_index(int_nt, IREF);
                uint htemp;

                INCR(find_name);
                if ((pvalue = dict_find_name_by_index_inline(nidx, htemp)) == 0)
                    return_with_error_iref(gs_error_undefined);
            }
             
             
            switch (r_type_xe(pvalue)) {
                  cases_invalid():
                    return_with_error_iref(gs_error_Fatal);
                  cases_nox():   
                    return_with_error_iref(gs_error_invalidaccess);
                  cases_lit_1():
                  cases_lit_2():
                  cases_lit_3():
                  cases_lit_4():
                  cases_lit_5():
                      INCR(name_lit);
                     
                    if (iosp >= ostop)
                        return_with_stackoverflow(pvalue);
                    ++iosp;
                    ref_assign_inline(iosp, pvalue);
                    next();
                case exec(t_array):
                case exec(t_mixedarray):
                case exec(t_shortarray):
                    INCR(name_proc);
                     
                    goto prst;
                case plain_exec(tx_op_add):
                    goto x_add;
                case plain_exec(tx_op_def):
                    goto x_def;
                case plain_exec(tx_op_dup):
                    goto x_dup;
                case plain_exec(tx_op_exch):
                    goto x_exch;
                case plain_exec(tx_op_if):
                    goto x_if;
                case plain_exec(tx_op_ifelse):
                    goto x_ifelse;
                case plain_exec(tx_op_index):
                    goto x_index;
                case plain_exec(tx_op_pop):
                    goto x_pop;
                case plain_exec(tx_op_roll):
                    goto x_roll;
                case plain_exec(tx_op_sub):
                    goto x_sub;
                case plain_exec(t_null):
                    goto bot;
                case plain_exec(t_oparray):
                    INCR(name_oparray);
                    opindex = op_index(pvalue);
                    pvalue = (ref *)pvalue->value.const_refs;
                    goto opst;
                case plain_exec(t_operator):
                    INCR(name_operator);
                    {            
                         
                        if (--(*ticks_left) <= 0) {         
                             
 
                        }
                        esp = iesp;
                        osp = iosp;
                        switch (code = call_operator(real_opproc(pvalue),
                                                     i_ctx_p)
                                ) {
                            case 0:      
                            case 1:      
                                iosp = osp;
                                next();
                            case o_push_estack:
                                store_state(iesp);
                                goto opush;
                            case o_pop_estack:
                                goto opop;
                            case o_reschedule:
                                store_state(iesp);
                                goto res;
                            case gs_error_Remap_Color:
                                goto oe_remap;
                        }
                        iosp = osp;
                        iesp = esp;
                        return_with_error(code, pvalue);
                    }
                case plain_exec(t_name):
                case exec(t_file):
                case exec(t_string):
                default:
                     
                    store_state(iesp);
                    icount = 0;
                    SET_IREF(pvalue);
                    goto top;
            }
        case exec(t_file):
            {    
                stream *s;
                scanner_state sstate;

                check_read_known_file(i_ctx_p, s, IREF, return_with_error_iref);
            rt:
                if (iosp >= ostop)       
                    return_with_stackoverflow_iref();
                osp = iosp;      
                gs_scanner_init_options(&sstate, IREF, i_ctx_p->scanner_options);
            again:
                code = gs_scan_token(i_ctx_p, &token, &sstate);
                iosp = osp;      
                switch (code) {
                    case 0:      
                         
                         
                         
                         
                         
                         
                        if (!r_has_attr(&token, a_executable) ||
                            r_is_array(&token)
                            ) {  
                             
                             
                            iosp++;
                            ref_assign_inline(iosp, &token);
                            goto rt;
                        }
                        store_state(iesp);
                         
                        if (iesp >= estop)
                            return_with_error_iref(gs_error_execstackoverflow);
                        esfile_set_cache(++iesp);
                        ref_assign_inline(iesp, IREF);
                        SET_IREF(&token);
                        icount = 0;
                        goto top;
                    case gs_error_undefined:    
                        gs_scanner_error_object(i_ctx_p, &sstate, &token);
                        return_with_error(code, &token);
                    case scan_EOF:       
                        esfile_clear_cache();
                        goto bot;
                    case scan_BOS:
                         
                         
                        store_state(iesp);
                         
                        if (iesp >= estop)
                            return_with_error_iref(gs_error_execstackoverflow);
                        esfile_set_cache(++iesp);
                        ref_assign_inline(iesp, IREF);
                        pvalue = &token;
                        goto pr;
                    case scan_Refill:
                        store_state(iesp);
                         
                         
                        ref_assign_inline(&token, IREF);
                         
                        if (iesp >= estop)
                            return_with_error_iref(gs_error_execstackoverflow);
                        ++iesp;
                        ref_assign_inline(iesp, &token);
                        esp = iesp;
                        osp = iosp;
                        code = gs_scan_handle_refill(i_ctx_p, &sstate, true,
                                                     ztokenexec_continue);
                scan_cont:
                        iosp = osp;
                        iesp = esp;
                        switch (code) {
                            case 0:
                                iesp--;          
                                goto again;      
                            case o_push_estack:
                                esfile_clear_cache();
                                if (--(*ticks_left) > 0)
                                    goto up;
                                goto slice;
                        }
                         
                        iesp--;  
                        return_with_code_iref();
                    case scan_Comment:
                    case scan_DSC_Comment: {
                         
                        ref file_token;

                        store_state(iesp);
                        ref_assign_inline(&file_token, IREF);
                        if (iesp >= estop)
                            return_with_error_iref(gs_error_execstackoverflow);
                        ++iesp;
                        ref_assign_inline(iesp, &file_token);
                        esp = iesp;
                        osp = iosp;
                        code = ztoken_handle_comment(i_ctx_p,
                                                     &sstate, &token,
                                                     code, true, true,
                                                     ztokenexec_continue);
                    }
                        goto scan_cont;
                    default:     
                        ref_assign_inline(&token, IREF);
                        gs_scanner_error_object(i_ctx_p, &sstate, &token);
                        return_with_error(code, &token);
                }
            }
        case exec(t_string):
            {                    
                stream ss;
                scanner_state sstate;

                s_init(&ss, NULL);
                sread_string(&ss, IREF->value.bytes, r_size(IREF));
                gs_scanner_init_stream_options(&sstate, &ss, SCAN_FROM_STRING);
                osp = iosp;      
                code = gs_scan_token(i_ctx_p, &token, &sstate);
                iosp = osp;      
                switch (code) {
                    case 0:      
                    case scan_BOS:       
                        store_state(iesp);
                         
                         
                        {
                            uint size = sbufavailable(&ss);

                            if (size) {
                                if (iesp >= estop)
                                    return_with_error_iref(gs_error_execstackoverflow);
                                ++iesp;
                                iesp->tas.type_attrs = IREF->tas.type_attrs;
                                iesp->value.const_bytes = sbufptr(&ss);
                                r_set_size(iesp, size);
                            }
                        }
                        if (code == 0) {
                            SET_IREF(&token);
                            icount = 0;
                            goto top;
                        }
                         
                        pvalue = &token;
                        goto pr;
                    case scan_EOF:       
                        goto bot;
                    case scan_Refill:    
                        code = gs_note_error(gs_error_syntaxerror);
                         
                    default:     
                        ref_assign_inline(&token, IREF);
                        gs_scanner_error_object(i_ctx_p, &sstate, &token);
                        return_with_error(code, &token);
                }
            }
             
             
        default:
            {
                uint index;

                switch (*iref_packed >> r_packed_type_shift) {
                    case pt_full_ref:
                    case pt_full_ref + 1:
                        INCR(p_full);
                        if (iosp >= ostop)
                            return_with_stackoverflow_iref();
                         
                         
                        ++iosp;
                         
                         
                        ref_assign_inline(iosp, IREF);
                        next();
                    case pt_executable_operator:
                        index = *iref_packed & packed_value_mask;
                        if (--(*ticks_left) <= 0) {         
                             
 
                        }
                        if (!op_index_is_operator(index)) {
                            INCR(p_exec_oparray);
                            store_state_short(iesp);
                            opindex = index;
                             
                            index -= op_def_count;
                            pvalue = (ref *)
                                (index < r_size(&i_ctx_p->op_array_table_global.table) ?
                                 i_ctx_p->op_array_table_global.table.value.const_refs +
                                 index :
                                 i_ctx_p->op_array_table_local.table.value.const_refs +
                                 (index - r_size(&i_ctx_p->op_array_table_global.table)));
                            goto oppr;
                        }
                        INCR(p_exec_operator);
                         
                         
#if PACKED_SPECIAL_OPS
                         
#  define case_xop(xop) case xop - (int)tx_op + 1
                        switch (index) {
                              case_xop(tx_op_add):goto x_add;
                              case_xop(tx_op_def):goto x_def;
                              case_xop(tx_op_dup):goto x_dup;
                              case_xop(tx_op_exch):goto x_exch;
                              case_xop(tx_op_if):goto x_if;
                              case_xop(tx_op_ifelse):goto x_ifelse;
                              case_xop(tx_op_index):goto x_index;
                              case_xop(tx_op_pop):goto x_pop;
                              case_xop(tx_op_roll):goto x_roll;
                              case_xop(tx_op_sub):goto x_sub;
                            case 0:      
                            default:
                                ;
                        }
#  undef case_xop
#endif
                        INCR(p_exec_non_x_operator);
                        esp = iesp;
                        osp = iosp;
                        switch (code = call_operator(op_index_proc(index), i_ctx_p)) {
                            case 0:
                            case 1:
                                iosp = osp;
                                next_short();
                            case o_push_estack:
                                store_state_short(iesp);
                                goto opush;
                            case o_pop_estack:
                                iosp = osp;
                                if (esp == iesp) {
                                    next_short();
                                }
                                iesp = esp;
                                goto up;
                            case o_reschedule:
                                store_state_short(iesp);
                                goto res;
                            case gs_error_Remap_Color:
                                store_state_short(iesp);
                                goto remap;
                        }
                        iosp = osp;
                        iesp = esp;
                        return_with_code_iref();
                    case pt_integer:
                        INCR(p_integer);
                        if (iosp >= ostop)
                            return_with_stackoverflow_iref();
                        ++iosp;
                        make_int(iosp,
                                 ((int)*iref_packed & packed_int_mask) +
                                 packed_min_intval);
                        next_short();
                    case pt_literal_name:
                        INCR(p_lit_name);
                        {
                            uint nidx = *iref_packed & packed_value_mask;

                            if (iosp >= ostop)
                                return_with_stackoverflow_iref();
                            ++iosp;
                            name_index_ref_inline(int_nt, nidx, iosp);
                            next_short();
                        }
                    case pt_executable_name:
                        INCR(p_exec_name);
                        {
                            uint nidx = *iref_packed & packed_value_mask;

                            pvalue = name_index_ptr_inline(int_nt, nidx)->pvalue;
                            if (!pv_valid(pvalue)) {
                                uint htemp;

                                INCR(p_find_name);
                                if ((pvalue = dict_find_name_by_index_inline(nidx, htemp)) == 0) {
                                    names_index_ref(int_nt, nidx, &token);
                                    return_with_error(gs_error_undefined, &token);
                                }
                            }
                            if (r_has_masked_attrs(pvalue, a_execute, a_execute + a_executable)) {       
                                INCR(p_name_lit);
                                if (iosp >= ostop)
                                    return_with_stackoverflow_iref();
                                ++iosp;
                                ref_assign_inline(iosp, pvalue);
                                next_short();
                            }
                            if (r_is_proc(pvalue)) {     
                                 
                                INCR(p_name_proc);
                                store_state_short(iesp);
                                goto pr;
                            }
                             
                            store_state_short(iesp);
                            icount = 0;
                            SET_IREF(pvalue);
                            goto top;
                        }
                         
                }
            }
    }
     
    if (iosp >= ostop)
        return_with_stackoverflow_iref();
    ++iosp;
    ref_assign_inline(iosp, IREF);
  bot:next();
  out:                           
     
    if (!icount) {
         
        iesp--;
        iref_packed = IREF_NEXT(iref_packed);
        goto top;
    }
  up:if (--(*ticks_left) < 0)
        goto slice;
     
    if (!r_is_proc(iesp)) {
        SET_IREF(iesp--);
        icount = 0;
        goto top;
    }
    SET_IREF(iesp->value.refs);  
    icount = r_size(iesp) - 1;
    if (icount <= 0) {           
        iesp--;                  
        if (icount < 0)
            goto up;
    }
    goto top;
res:
     
     
    *pi_ctx_p = i_ctx_p;
    code = (*i_ctx_p->reschedule_proc)(pi_ctx_p);
    i_ctx_p = *pi_ctx_p;
  sched:                         
     
    if (code < 0) {
        set_error(code);
         
        make_null_proc(&ierror.full);
        SET_IREF(ierror.obj = &ierror.full);
        goto error_exit;
    }
     
    iosp = osp;
    iesp = esp;
    goto up;
#if 0                            
  sst:                           
    store_state(iesp);
    if (iesp >= estop)
        return_with_error_iref(gs_error_execstackoverflow);
    iesp++;
    ref_assign_inline(iesp, iref);
#endif  
  slice:                         
     
    osp = iosp;
    esp = iesp;
     
    if ((*ticks_left) <= -100) {    
        *pi_ctx_p = i_ctx_p;
        code = interp_reclaim(pi_ctx_p, -1);
        i_ctx_p = *pi_ctx_p;
    } else if (i_ctx_p->time_slice_proc != NULL) {
        *pi_ctx_p = i_ctx_p;
        code = (*i_ctx_p->time_slice_proc)(pi_ctx_p);
        i_ctx_p = *pi_ctx_p;
    } else
        code = 0;
    *ticks_left = i_ctx_p->time_slice_ticks;
    set_code_on_interrupt(imemory, &code);
    goto sched;

     

  rweci:
    ierror.code = code;
  rwei:
    ierror.obj = IREF;
  rwe:
    if (!r_is_packed(iref_packed))
        store_state(iesp);
    else {
         
        packed_get(imemory, (const ref_packed *)ierror.obj, &ierror.full);
        store_state_short(iesp);
        if (IREF == ierror.obj)
            SET_IREF(&ierror.full);
        ierror.obj = &ierror.full;
    }
  error_exit:
    if (GS_ERROR_IS_INTERRUPT(ierror.code)) {       
         
         
         
         
        if (iesp >= estop)
            ierror.code = gs_error_execstackoverflow;
        else {
            iesp++;
            ref_assign_inline(iesp, IREF);
        }
    }
    esp = iesp;
    osp = iosp;
    ref_assign_inline(perror_object, ierror.obj);
#ifdef DEBUG
    if (ierror.code == gs_error_InterpreterExit) {
         
        return gs_error_InterpreterExit;
    }
#endif
    return gs_log_error(ierror.code, __FILE__, ierror.line);
}

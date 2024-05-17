int gs_main_init2aux(gs_main_instance * minst) {
    i_ctx_t * i_ctx_p = minst->i_ctx_p;

    if (minst->init_done < 2) {
        int code, exit_code;
        ref error_object, ifa;

         
        i_ctx_p->op_array_table_global = empty_table;
        i_ctx_p->op_array_table_local = empty_table;

        code = zop_init(i_ctx_p);
        if (code < 0)
            return code;
        code = op_init(i_ctx_p);         
        if (code < 0)
            return code;

         
        make_const_string(&ifa, a_readonly | avm_foreign, gs_init_files_sizeof - 2, gs_init_files);
        code = initial_enter_name("INITFILES", &ifa);
        if (code < 0)
            return code;

         
        make_const_string(&ifa, a_readonly | avm_foreign, gs_emulators_sizeof - 2, gs_emulators);
        code = initial_enter_name("EMULATORS", &ifa);
        if (code < 0)
            return code;

         
        code = initial_enter_name("LIBPATH", &minst->lib_path.list);
        if (code < 0)
            return code;

         
        code = gs_run_init_file(minst, &exit_code, &error_object);
        if (code < 0)
            return code;
        minst->init_done = 2;
         
        if (minst->display)
        if ((code = display_set_callback(minst, minst->display)) < 0)
            return code;

        if ((code = gs_main_run_string(minst,
                "JOBSERVER "
                " { false 0 .startnewjob } "
                " { NOOUTERSAVE not { save pop } if } "
                "ifelse", 0, &exit_code,
                &error_object)) < 0)
           return code;
    }
    return 0;
}

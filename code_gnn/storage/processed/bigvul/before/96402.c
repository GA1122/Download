char *get_backtrace(const char *dump_dir_name, unsigned timeout_sec, const char *debuginfo_dirs)
{
    INITIALIZE_LIBABRT();

    struct dump_dir *dd = dd_opendir(dump_dir_name,   0);
    if (!dd)
        return NULL;
    char *executable = dd_load_text(dd, FILENAME_EXECUTABLE);
    dd_close(dd);

     
    log(_("Generating backtrace"));

    unsigned i = 0;
    char *args[25];
    args[i++] = (char*)"gdb";
    args[i++] = (char*)"-batch";
    struct strbuf *set_debug_file_directory = strbuf_new();
    unsigned auto_load_base_index = 0;
    if(debuginfo_dirs == NULL)
    {
        strbuf_append_str(set_debug_file_directory, "set debug-file-directory /");
    }
    else
    {
        strbuf_append_str(set_debug_file_directory, "set debug-file-directory /usr/lib/debug");

        struct strbuf *debug_directories = strbuf_new();
        const char *p = debuginfo_dirs;
        while (1)
        {
            while (*p == ':')
                p++;
            if (*p == '\0')
                break;
            const char *colon_or_nul = strchrnul(p, ':');
            strbuf_append_strf(debug_directories, "%s%.*s/usr/lib/debug", (debug_directories->len == 0 ? "" : ":"),
                                                                          (int)(colon_or_nul - p), p);
            p = colon_or_nul;
        }

        strbuf_append_strf(set_debug_file_directory, ":%s", debug_directories->buf);

        args[i++] = (char*)"-iex";
        auto_load_base_index = i;
        args[i++] = xasprintf("add-auto-load-safe-path %s", debug_directories->buf);
        args[i++] = (char*)"-iex";
        args[i++] = xasprintf("add-auto-load-scripts-directory %s", debug_directories->buf);

        strbuf_free(debug_directories);
    }

    args[i++] = (char*)"-ex";
    const unsigned debug_dir_cmd_index = i++;
    args[debug_dir_cmd_index] = strbuf_free_nobuf(set_debug_file_directory);

     
    args[i++] = (char*)"-ex";
    const unsigned file_cmd_index = i++;
    args[file_cmd_index] = xasprintf("file %s", executable);
    free(executable);

    args[i++] = (char*)"-ex";
    const unsigned core_cmd_index = i++;
    args[core_cmd_index] = xasprintf("core-file %s/"FILENAME_COREDUMP, dump_dir_name);

    args[i++] = (char*)"-ex";
    const unsigned bt_cmd_index = i++;
     
    args[i++] = (char*)"-ex";
    args[i++] = (char*)"info sharedlib";
     
    args[i++] = (char*)"-ex";
    args[i++] = (char*)"print (char*)__abort_msg";
    args[i++] = (char*)"-ex";
    args[i++] = (char*)"print (char*)__glib_assert_msg";
    args[i++] = (char*)"-ex";
    args[i++] = (char*)"info all-registers";
    args[i++] = (char*)"-ex";
    const unsigned dis_cmd_index = i++;
    args[dis_cmd_index] = (char*)"disassemble";
    args[i++] = NULL;

     
     
    unsigned bt_depth = 1024;
    const char *thread_apply_all = "thread apply all";
    const char *full = " full";
    char *bt = NULL;
    while (1)
    {
        args[bt_cmd_index] = xasprintf("%s backtrace %u%s", thread_apply_all, bt_depth, full);
        bt = exec_vp(args,   1, timeout_sec, NULL);
        free(args[bt_cmd_index]);
        if ((bt && strnlen(bt, 256*1024) < 256*1024) || bt_depth <= 32)
        {
            break;
        }

        bt_depth /= 2;
        if (bt)
            log("Backtrace is too big (%u bytes), reducing depth to %u",
                        (unsigned)strlen(bt), bt_depth);
        else
             
            log("Failed to generate backtrace, reducing depth to %u",
                        bt_depth);
        free(bt);

         
        args[dis_cmd_index] = (char*)"disassemble $pc-20, $pc+64";

        if (bt_depth <= 64 && thread_apply_all[0] != '\0')
        {
             
            bt_depth = 128;
            thread_apply_all = "";
        }
        if (bt_depth <= 64 && full[0] != '\0')
        {
             
            bt_depth = 128;
            full = "";
        }
    }

    if (auto_load_base_index > 0)
    {
        free(args[auto_load_base_index]);
        free(args[auto_load_base_index + 2]);
    }

    free(args[debug_dir_cmd_index]);
    free(args[file_cmd_index]);
    free(args[core_cmd_index]);
    return bt;
}

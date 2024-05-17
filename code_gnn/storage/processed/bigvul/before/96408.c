static void save_oops_data_in_dump_dir(struct dump_dir *dd, char *oops, const char *proc_modules)
{
    char *first_line = oops;
    char *second_line = (char*)strchr(first_line, '\n');  
    *second_line++ = '\0';

    if (first_line[0])
        dd_save_text(dd, FILENAME_KERNEL, first_line);
    dd_save_text(dd, FILENAME_BACKTRACE, second_line);

     
    if (strstr(second_line, "Your BIOS is broken"))
        dd_save_text(dd, FILENAME_NOT_REPORTABLE,
                _("A kernel problem occurred because of broken BIOS. "
                  "Unfortunately, such problems are not fixable by kernel maintainers."));
     
    else if (strstr(second_line, "Your hardware is unsupported"))
        dd_save_text(dd, FILENAME_NOT_REPORTABLE,
                _("A kernel problem occurred, but your hardware is unsupported, "
                  "therefore kernel maintainers are unable to fix this problem."));
    else
    {
        char *tainted_short = kernel_tainted_short(second_line);
        if (tainted_short)
        {
            log_notice("Kernel is tainted '%s'", tainted_short);
            dd_save_text(dd, FILENAME_TAINTED_SHORT, tainted_short);

            char *tnt_long = kernel_tainted_long(tainted_short);
            dd_save_text(dd, FILENAME_TAINTED_LONG, tnt_long);
            free(tnt_long);

            struct strbuf *reason = strbuf_new();
            const char *fmt = _("A kernel problem occurred, but your kernel has been "
                    "tainted (flags:%s). Kernel maintainers are unable to "
                    "diagnose tainted reports.");
            strbuf_append_strf(reason, fmt, tainted_short);

            char *modlist = !proc_modules ? NULL : list_of_tainted_modules(proc_modules);
            if (modlist)
            {
                strbuf_append_strf(reason, _(" Tainted modules: %s."), modlist);
                free(modlist);
            }

            dd_save_text(dd, FILENAME_NOT_REPORTABLE, reason->buf);
            strbuf_free(reason);
            free(tainted_short);
        }
    }

    strchrnul(second_line, '\n')[0] = '\0';
    dd_save_text(dd, FILENAME_REASON, second_line);
}
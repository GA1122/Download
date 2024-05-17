 static int handle_parse_opts(QemuOpts *opts, struct FsDriverEntry *fse)
 {
     const char *sec_model = qemu_opt_get(opts, "security_model");
        error_report("fsdev: No path specified");
        return -1;
    }

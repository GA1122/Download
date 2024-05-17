void ssh_connshare_log(Ssh ssh, int event, const char *logtext,
                       const char *ds_err, const char *us_err)
{
    if (event == SHARE_NONE) {
         
        if (logtext) {
            logeventf(ssh, "Could not set up connection sharing: %s", logtext);
        } else {
            if (ds_err)
                logeventf(ssh, "Could not set up connection sharing"
                          " as downstream: %s", ds_err);
            if (us_err)
                logeventf(ssh, "Could not set up connection sharing"
                          " as upstream: %s", us_err);
        }
    } else if (event == SHARE_DOWNSTREAM) {
         
        logeventf(ssh, "Using existing shared connection at %s", logtext);
         
        if ((flags & FLAG_VERBOSE) || (flags & FLAG_INTERACTIVE)) {
            c_write_str(ssh,"Reusing a shared connection to this server.\r\n");
        }
    } else if (event == SHARE_UPSTREAM) {
         
        logeventf(ssh, "Sharing this connection at %s", logtext);
    }
}

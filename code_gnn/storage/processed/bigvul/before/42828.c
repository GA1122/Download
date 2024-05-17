static bool is_screencast_available()
{
    const char *args[3];
    args[0] = (char *) "fros";
    args[1] = "--is-available";
    args[2] = NULL;

    pid_t castapp = 0;
    castapp = fork_execv_on_steroids(
                EXECFLG_QUIET,
                (char **)args,
                NULL,
                  NULL,
                g_dump_dir_name,
                  0
    );

    int status;
    safe_waitpid(castapp, &status, 0);

     
    return status == 0;
}

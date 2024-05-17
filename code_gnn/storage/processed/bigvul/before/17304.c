struct vdagent_file_xfers *vdagent_file_xfers_create(
    struct udscs_connection *vdagentd, const char *save_dir,
    int open_save_dir, int debug)
{
    struct vdagent_file_xfers *xfers;

    xfers = g_malloc(sizeof(*xfers));
    xfers->xfers = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                                         NULL, vdagent_file_xfer_task_free);
    xfers->vdagentd = vdagentd;
    xfers->save_dir = g_strdup(save_dir);
    xfers->open_save_dir = open_save_dir;
    xfers->debug = debug;

    return xfers;
}

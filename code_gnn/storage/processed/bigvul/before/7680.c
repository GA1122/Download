static int handle_name_to_path(FsContext *ctx, V9fsPath *dir_path,
                              const char *name, V9fsPath *target)
{
    char *buffer;
    struct file_handle *fh;
    int dirfd, ret, mnt_id;
    struct handle_data *data = (struct handle_data *)ctx->private;

     
    if (!strcmp(name, ".") || !strcmp(name, "..")) {
        errno = EINVAL;
        return -1;

    }
    if (dir_path) {
        dirfd = open_by_handle(data->mountfd, dir_path->data, O_PATH);
    } else {
         
        buffer = rpath(ctx, ".");
        dirfd = open(buffer, O_DIRECTORY);
        g_free(buffer);
    }
    if (dirfd < 0) {
        return dirfd;
    }
    fh = g_malloc(sizeof(struct file_handle) + data->handle_bytes);
    fh->handle_bytes = data->handle_bytes;
     
    buffer = g_strdup_printf("./%s", name);
     
    ret = name_to_handle(dirfd, buffer, fh, &mnt_id, 0);
    if (!ret) {
        target->data = (char *)fh;
        target->size = sizeof(struct file_handle) + data->handle_bytes;
    } else {
        g_free(fh);
    }
    close(dirfd);
    g_free(buffer);
    return ret;
}

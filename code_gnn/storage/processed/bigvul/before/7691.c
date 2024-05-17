static int handle_renameat(FsContext *ctx, V9fsPath *olddir,
                           const char *old_name, V9fsPath *newdir,
                           const char *new_name)
{
    int olddirfd, newdirfd, ret;
    struct handle_data *data = (struct handle_data *)ctx->private;

    olddirfd = open_by_handle(data->mountfd, olddir->data, O_PATH);
    if (olddirfd < 0) {
        return olddirfd;
    }
    newdirfd = open_by_handle(data->mountfd, newdir->data, O_PATH);
    if (newdirfd < 0) {
        close(olddirfd);
        return newdirfd;
    }
    ret = renameat(olddirfd, old_name, newdirfd, new_name);
    close(newdirfd);
    close(olddirfd);
    return ret;
}

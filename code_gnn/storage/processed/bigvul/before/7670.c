static int handle_init(FsContext *ctx)
{
    int ret, mnt_id;
    struct statfs stbuf;
    struct file_handle fh;
    struct handle_data *data = g_malloc(sizeof(struct handle_data));

    data->mountfd = open(ctx->fs_root, O_DIRECTORY);
    if (data->mountfd < 0) {
        ret = data->mountfd;
        goto err_out;
    }
    ret = statfs(ctx->fs_root, &stbuf);
    if (!ret) {
        switch (stbuf.f_type) {
        case EXT2_SUPER_MAGIC:
        case BTRFS_SUPER_MAGIC:
        case REISERFS_SUPER_MAGIC:
        case XFS_SUPER_MAGIC:
            ctx->exops.get_st_gen = handle_ioc_getversion;
            break;
        }
    }
    memset(&fh, 0, sizeof(struct file_handle));
    ret = name_to_handle(data->mountfd, ".", &fh, &mnt_id, 0);
    if (ret && errno == EOVERFLOW) {
        data->handle_bytes = fh.handle_bytes;
        ctx->private = data;
        ret = 0;
        goto out;
    }
     
    ret = -1;
    close(data->mountfd);
err_out:
    g_free(data);
out:
     return ret;
 }

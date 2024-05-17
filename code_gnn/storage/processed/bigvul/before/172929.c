static void print_type(char *type, struct pid_info_t* info)
{
 static ssize_t link_dest_size;
 static char link_dest[PATH_MAX];

    strlcat(info->path, type, sizeof(info->path));
 if ((link_dest_size = readlink(info->path, link_dest, sizeof(link_dest)-1)) < 0) {
 if (errno == ENOENT)
 goto out;

        snprintf(link_dest, sizeof(link_dest), "%s (readlink: %s)", info->path, strerror(errno));
 } else {
        link_dest[link_dest_size] = '\0';
 }

 if (!strcmp(link_dest, "/"))
 goto out;

    printf("%-9s %5d %10s %4s %9s %18s %9s %10s %s\n",
            info->cmdline, info->pid, info->user, type,
 "???", "???", "???", "???", link_dest);

out:
    info->path[info->parent_length] = '\0';
}

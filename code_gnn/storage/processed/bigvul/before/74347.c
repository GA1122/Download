static int open_user_core(uid_t uid, uid_t fsuid, pid_t pid, char **percent_values)
{
    errno = 0;
    if (user_pwd == NULL
     || chdir(user_pwd) != 0
    ) {
        perror_msg("Can't cd to '%s'", user_pwd);
        return -1;
    }

    struct passwd* pw = getpwuid(uid);
    gid_t gid = pw ? pw->pw_gid : uid;
    xsetegid(gid);
    xseteuid(fsuid);

    if (strcmp(core_basename, "core") == 0)
    {
         
        char buf[] = "0\n";
        int fd = open("/proc/sys/kernel/core_uses_pid", O_RDONLY);
        if (fd >= 0)
        {
            IGNORE_RESULT(read(fd, buf, sizeof(buf)));
            close(fd);
        }
        if (strcmp(buf, "1\n") == 0)
        {
            core_basename = xasprintf("%s.%lu", core_basename, (long)pid);
        }
    }
    else
    {
         
        core_basename = xstrdup(core_basename);
        unsigned idx = 0;
        while (1)
        {
            char c = core_basename[idx];
            if (!c)
                break;
            idx++;
            if (c != '%')
                continue;

             
            c = core_basename[idx];
            unsigned specifier_num = strchrnul(percent_specifiers, c) - percent_specifiers;
            if (percent_specifiers[specifier_num] != '\0')  
            {
                const char *val = "%";
                if (specifier_num > 0)  
                    val = percent_values[specifier_num - 1];

                 
                idx--;
                char *old = core_basename;
                core_basename = xasprintf("%.*s%s%s", idx, core_basename, val, core_basename + idx + 2);
                free(old);
                idx += strlen(val);
            }
             
        }
    }

    full_core_basename = core_basename;
    if (core_basename[0] != '/')
        core_basename = concat_path_file(user_pwd, core_basename);

     
    struct stat sb;
    errno = 0;
     
    int user_core_fd = open(core_basename, O_WRONLY | O_CREAT | O_NOFOLLOW, 0600);  
    xsetegid(0);
    xseteuid(0);
    if (user_core_fd < 0
     || fstat(user_core_fd, &sb) != 0
     || !S_ISREG(sb.st_mode)
     || sb.st_nlink != 1
     
    ) {
        if (user_core_fd < 0)
            perror_msg("Can't open '%s'", full_core_basename);
        else
            perror_msg("'%s' is not a regular file with link count 1", full_core_basename);
        return -1;
    }
    if (ftruncate(user_core_fd, 0) != 0) {
         
        perror_msg("Can't truncate '%s' to size 0", full_core_basename);
        unlink(core_basename);
        return -1;
    }

    return user_core_fd;
}

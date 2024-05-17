int low_free_space(unsigned setting_MaxCrashReportsSize, const char *dump_location)
{
    struct statvfs vfs;
    if (statvfs(dump_location, &vfs) != 0)
    {
        perror_msg("statvfs('%s')", dump_location);
        return 0;
    }

     

     
    unsigned long fs_free_mb_x4 = ((unsigned long long)vfs.f_bfree / (1024/4)) * vfs.f_bsize / 1024;
    if (fs_free_mb_x4 < setting_MaxCrashReportsSize)
    {
        error_msg("Only %luMiB is available on %s",
                          fs_free_mb_x4 / 4, dump_location);
        return 1;
    }
    return 0;
}
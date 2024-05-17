auth_etc_enlightenment_sysactions_perm(char *path)
{
   struct stat st;
   if (stat(path, &st) == -1)
     return;

   if ((st.st_mode & S_IWGRP) || (st.st_mode & S_IXGRP) ||
       (st.st_mode & S_IWOTH) || (st.st_mode & S_IXOTH))
     {
        printf("ERROR: CONFIGURATION FILE HAS BAD PERMISSIONS (writable by group and/or others)\n");
        exit(10);
     }
}

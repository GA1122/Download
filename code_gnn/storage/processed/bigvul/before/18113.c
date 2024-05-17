mount_args_check(int argc, char **argv, const char *action)
{
   Eina_Bool opts = EINA_FALSE;
   struct stat st;
   const char *node;
   char buf[PATH_MAX];

   if (!strcmp(action, "mount"))
     {
         
        if (argc != 6) return EINA_FALSE;
        if (argv[2][0] == '-')
          {
              
             if (strcmp(argv[2], "-o")) return EINA_FALSE;
             opts = mountopts_check(argv[3]);
          }
        if (!opts) return EINA_FALSE;
        if (!strncmp(argv[4], "UUID=", sizeof("UUID=") - 1))
          {
             if (!check_uuid(argv[4] + 5)) return EINA_FALSE;
          }
        else
          {
             if (strncmp(argv[4], "/dev/", 5)) return EINA_FALSE;
             if (stat(argv[4], &st)) return EINA_FALSE;
          }

        node = strrchr(argv[5], '/');
        if (!node) return EINA_FALSE;
        if (!node[1]) return EINA_FALSE;
        if (node - argv[5] != 6) return EINA_FALSE;
        snprintf(buf, sizeof(buf), "/dev%s", node);
        if (stat(buf, &st)) return EINA_FALSE;
     }
   else if (!strcmp(action, "umount"))
     {
         
        if (argc != 3) return EINA_FALSE;
        if (strncmp(argv[2], "/dev/", 5)) return EINA_FALSE;
        if (stat(argv[2], &st)) return EINA_FALSE;
        node = strrchr(argv[2], '/');
        if (!node) return EINA_FALSE;
        if (!node[1]) return EINA_FALSE;
        if (node - argv[2] != 4) return EINA_FALSE;
         
     }
   else if (!strcmp(action, "eject"))
     {
         
        if (argc != 3) return EINA_FALSE;
        if (strncmp(argv[2], "/dev/", 5)) return EINA_FALSE;
        if (stat(argv[2], &st)) return EINA_FALSE;
        node = strrchr(argv[2], '/');
        if (!node) return EINA_FALSE;
        if (!node[1]) return EINA_FALSE;
        if (node - argv[2] != 4) return EINA_FALSE;
     }
   else return EINA_FALSE;
   return EINA_TRUE;
}

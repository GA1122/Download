_cserve2_start()
{
   pid_t cs_child;
   cs_child = fork();
   if (cs_child == 0)
     {
        char *cs_args[2] = { NULL, NULL };

        cs_args[0] = (char *)evas_cserve_path_get();
        execv(cs_args[0], cs_args);
        exit(-1);
     }
   else if (cs_child > 0)
     {
        putenv("EVAS_CSERVE2=1");
     }
   else
     {
        unsetenv("EVAS_CSERVE2");
     }
   return cs_child;
}

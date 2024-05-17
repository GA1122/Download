static int create_cmdjob(char *cmd)
{
 pthread_t thread_id;
 char *job_cmd;

    job_cmd = malloc(strlen(cmd)+1);  
    strcpy(job_cmd, cmd);

 if (pthread_create(&thread_id, NULL,
 (void*)cmdjob_handler, (void*)job_cmd)!=0)
      perror("pthread_create");

 return 0;
}

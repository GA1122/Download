int main (int UNUSED argc, char UNUSED *argv[])
{
 int opt;
 char cmd[128];
 int args_processed = 0;
 int pid = -1;

    config_permissions();
    bdt_log("\n:::::::::::::::::::::::::::::::::::::::::::::::::::");
    bdt_log(":: Bluedroid test app starting");

 if ( HAL_load() < 0 ) {
        perror("HAL failed to initialize, exit\n");
        unlink(PID_FILE);
        exit(0);
 }

    setup_test_env();

  
    bdt_init();

 while(!main_done)
 {
 char line[128];

  
        printf( ">" );
        fflush(stdout);

        fgets (line, 128, stdin);

 if (line[0]!= '\0')
 {
  
            line[strlen(line)-1] = 0;

            process_cmd(line, 0);
            memset(line, '\0', 128);
 }
 }

  

    HAL_unload();

    bdt_log(":: Bluedroid test app terminating");

 return 0;
}

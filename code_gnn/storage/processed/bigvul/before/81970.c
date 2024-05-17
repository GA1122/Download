static int issueCommandRepeat(int argc, char **argv, long repeat) {
    while (1) {
        config.cluster_reissue_command = 0;
        if (cliSendCommand(argc,argv,repeat) != REDIS_OK) {
            cliConnect(1);

             
            if (cliSendCommand(argc,argv,repeat) != REDIS_OK) {
                cliPrintContextError();
                return REDIS_ERR;
            }
         }
          
         if (config.cluster_mode && config.cluster_reissue_command) {
            cliConnect(1);
         } else {
             break;
        }
    }
    return REDIS_OK;
}
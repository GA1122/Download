void databasesCron(void) {
     
    if (server.active_expire_enabled && server.masterhost == NULL)
        activeExpireCycle(ACTIVE_EXPIRE_CYCLE_SLOW);

     
    if (server.rdb_child_pid == -1 && server.aof_child_pid == -1) {
         
        static unsigned int resize_db = 0;
        static unsigned int rehash_db = 0;
        int dbs_per_call = CRON_DBS_PER_CALL;
        int j;

         
        if (dbs_per_call > server.dbnum) dbs_per_call = server.dbnum;

         
        for (j = 0; j < dbs_per_call; j++) {
            tryResizeHashTables(resize_db % server.dbnum);
            resize_db++;
        }

         
        if (server.activerehashing) {
            for (j = 0; j < dbs_per_call; j++) {
                int work_done = incrementallyRehash(rehash_db % server.dbnum);
                rehash_db++;
                if (work_done) {
                     
                    break;
                }
            }
        }
    }
}

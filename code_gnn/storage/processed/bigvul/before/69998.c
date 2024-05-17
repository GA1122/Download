void beforeSleep(struct aeEventLoop *eventLoop) {
    UNUSED(eventLoop);

     
    if (server.cluster_enabled) clusterBeforeSleep();

     
    if (server.active_expire_enabled && server.masterhost == NULL)
        activeExpireCycle(ACTIVE_EXPIRE_CYCLE_FAST);

     
    if (server.get_ack_from_slaves) {
        robj *argv[3];

        argv[0] = createStringObject("REPLCONF",8);
        argv[1] = createStringObject("GETACK",6);
        argv[2] = createStringObject("*",1);  
        replicationFeedSlaves(server.slaves, server.slaveseldb, argv, 3);
        decrRefCount(argv[0]);
        decrRefCount(argv[1]);
        decrRefCount(argv[2]);
        server.get_ack_from_slaves = 0;
    }

     
    if (listLength(server.clients_waiting_acks))
        processClientsWaitingReplicas();

     
    if (listLength(server.unblocked_clients))
        processUnblockedClients();

     
    flushAppendOnlyFile(0);

     
    handleClientsWithPendingWrites();
}

SmartScheduleClient(int *clientReady, int nready)
{
    ClientPtr pClient;
    int i;
    int client;
    int bestPrio, best = 0;
    int bestRobin, robin;
    long now = SmartScheduleTime;
    long idle;

    bestPrio = -0x7fffffff;
    bestRobin = 0;
    idle = 2 * SmartScheduleSlice;
    for (i = 0; i < nready; i++) {
        client = clientReady[i];
        pClient = clients[client];
         
        if ((now - pClient->smart_stop_tick) >= idle) {
            if (pClient->smart_priority < 0)
                pClient->smart_priority++;
        }

         
        robin =
            (pClient->index -
             SmartLastIndex[pClient->smart_priority -
                            SMART_MIN_PRIORITY]) & 0xff;
        if (pClient->smart_priority > bestPrio ||
            (pClient->smart_priority == bestPrio && robin > bestRobin)) {
            bestPrio = pClient->smart_priority;
            bestRobin = robin;
            best = client;
        }
#ifdef SMART_DEBUG
        if ((now - SmartLastPrint) >= 5000)
            fprintf(stderr, " %2d: %3d", client, pClient->smart_priority);
#endif
    }
#ifdef SMART_DEBUG
    if ((now - SmartLastPrint) >= 5000) {
        fprintf(stderr, " use %2d\n", best);
        SmartLastPrint = now;
    }
#endif
    pClient = clients[best];
    SmartLastIndex[bestPrio - SMART_MIN_PRIORITY] = pClient->index;
     
    if (SmartLastClient != pClient) {
        pClient->smart_start_tick = now;
        SmartLastClient = pClient;
    }
     
    if (nready == 1 && SmartScheduleLatencyLimited == 0) {
         
        if ((now - pClient->smart_start_tick) > 1000 &&
            SmartScheduleSlice < SmartScheduleMaxSlice) {
            SmartScheduleSlice += SmartScheduleInterval;
        }
    }
    else {
        SmartScheduleSlice = SmartScheduleInterval;
    }
    return best;
}

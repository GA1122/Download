static void latencyDistMode(void) {
    redisReply *reply;
    long long start, latency, count = 0;
    long long history_interval =
        config.interval ? config.interval/1000 :
                          LATENCY_DIST_DEFAULT_INTERVAL;
    long long history_start = ustime();
    int j, outputs = 0;

    struct distsamples samples[] = {
         
        {10,0,'.'},          
        {125,0,'-'},         
        {250,0,'*'},         
        {500,0,'#'},         
        {1000,0,'1'},        
        {2000,0,'2'},        
        {3000,0,'3'},        
        {4000,0,'4'},        
        {5000,0,'5'},        
        {6000,0,'6'},        
        {7000,0,'7'},        
        {8000,0,'8'},        
        {9000,0,'9'},        
        {10000,0,'A'},       
        {20000,0,'B'},       
        {30000,0,'C'},       
        {40000,0,'D'},       
        {50000,0,'E'},       
        {100000,0,'F'},      
        {200000,0,'G'},      
        {300000,0,'H'},      
        {400000,0,'I'},      
        {500000,0,'J'},      
        {1000000,0,'K'},     
        {2000000,0,'L'},     
        {4000000,0,'M'},     
        {8000000,0,'N'},     
        {16000000,0,'O'},    
        {30000000,0,'P'},    
        {60000000,0,'Q'},    
        {0,0,'?'},           
    };

    if (!context) exit(1);
    while(1) {
        start = ustime();
        reply = reconnectingRedisCommand(context,"PING");
        if (reply == NULL) {
            fprintf(stderr,"\nI/O error\n");
            exit(1);
        }
        latency = ustime()-start;
        freeReplyObject(reply);
        count++;

         
        for (j = 0; ; j++) {
            if (samples[j].max == 0 || latency <= samples[j].max) {
                samples[j].count++;
                break;
            }
        }

         
        if (count && (ustime()-history_start)/1000 > history_interval) {
            if ((outputs++ % 20) == 0)
                showLatencyDistLegend();
            showLatencyDistSamples(samples,count);
            history_start = ustime();
            count = 0;
        }
        usleep(LATENCY_SAMPLE_RATE * 1000);
    }
}

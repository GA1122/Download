static void findBigKeys(void) {
    unsigned long long biggest[TYPE_COUNT] = {0}, counts[TYPE_COUNT] = {0}, totalsize[TYPE_COUNT] = {0};
    unsigned long long sampled = 0, total_keys, totlen=0, *sizes=NULL, it=0;
    sds maxkeys[TYPE_COUNT] = {0};
    char *typename[] = {"string","list","set","hash","zset","stream","none"};
    char *typeunit[] = {"bytes","items","members","fields","members","entries",""};
    redisReply *reply, *keys;
    unsigned int arrsize=0, i;
    int type, *types=NULL;
    double pct;

     
    total_keys = getDbSize();

     
    printf("\n# Scanning the entire keyspace to find biggest keys as well as\n");
    printf("# average sizes per key type.  You can use -i 0.1 to sleep 0.1 sec\n");
    printf("# per 100 SCAN commands (not usually needed).\n\n");

     
    for(i=0;i<TYPE_NONE; i++) {
        maxkeys[i] = sdsempty();
        if(!maxkeys[i]) {
            fprintf(stderr, "Failed to allocate memory for largest key names!\n");
            exit(1);
        }
    }

     
    do {
         
        pct = 100 * (double)sampled/total_keys;

         
        reply = sendScan(&it);
        keys  = reply->element[1];

         
        if(keys->elements > arrsize) {
            types = zrealloc(types, sizeof(int)*keys->elements);
            sizes = zrealloc(sizes, sizeof(unsigned long long)*keys->elements);

            if(!types || !sizes) {
                fprintf(stderr, "Failed to allocate storage for keys!\n");
                exit(1);
            }

            arrsize = keys->elements;
        }

         
        getKeyTypes(keys, types);
        getKeySizes(keys, types, sizes);

         
        for(i=0;i<keys->elements;i++) {
            if((type = types[i]) == TYPE_NONE)
                continue;

            totalsize[type] += sizes[i];
            counts[type]++;
            totlen += keys->element[i]->len;
            sampled++;

            if(biggest[type]<sizes[i]) {
                printf(
                   "[%05.2f%%] Biggest %-6s found so far '%s' with %llu %s\n",
                   pct, typename[type], keys->element[i]->str, sizes[i],
                   typeunit[type]);

                 
                maxkeys[type] = sdscpy(maxkeys[type], keys->element[i]->str);
                if(!maxkeys[type]) {
                    fprintf(stderr, "Failed to allocate memory for key!\n");
                    exit(1);
                }

                 
                biggest[type] = sizes[i];
            }

             
            if(sampled % 1000000 == 0) {
                printf("[%05.2f%%] Sampled %llu keys so far\n", pct, sampled);
            }
        }

         
        if(sampled && (sampled %100) == 0 && config.interval) {
            usleep(config.interval);
        }

        freeReplyObject(reply);
    } while(it != 0);

    if(types) zfree(types);
    if(sizes) zfree(sizes);

     
    printf("\n-------- summary -------\n\n");

    printf("Sampled %llu keys in the keyspace!\n", sampled);
    printf("Total key length in bytes is %llu (avg len %.2f)\n\n",
       totlen, totlen ? (double)totlen/sampled : 0);

     
    for(i=0;i<TYPE_NONE;i++) {
        if(sdslen(maxkeys[i])>0) {
            printf("Biggest %6s found '%s' has %llu %s\n", typename[i], maxkeys[i],
               biggest[i], typeunit[i]);
        }
    }

    printf("\n");

    for(i=0;i<TYPE_NONE;i++) {
        printf("%llu %ss with %llu %s (%05.2f%% of keys, avg size %.2f)\n",
           counts[i], typename[i], totalsize[i], typeunit[i],
           sampled ? 100 * (double)counts[i]/sampled : 0,
           counts[i] ? (double)totalsize[i]/counts[i] : 0);
    }

     
    for(i=0;i<TYPE_NONE;i++) {
        sdsfree(maxkeys[i]);
    }

     
    exit(0);
}
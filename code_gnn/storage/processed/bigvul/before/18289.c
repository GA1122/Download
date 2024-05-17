static void settings_init(void) {
    settings.use_cas = true;
    settings.access = 0700;
    settings.port = 11211;
    settings.udpport = 11211;
     
    settings.inter = NULL;
    settings.maxbytes = 64 * 1024 * 1024;  
    settings.maxconns = 1024;          
    settings.verbose = 0;
    settings.oldest_live = 0;
    settings.evict_to_free = 1;        
    settings.socketpath = NULL;        
    settings.factor = 1.25;
    settings.chunk_size = 48;          
    settings.num_threads = 4;          
    settings.prefix_delimiter = ':';
    settings.detail_enabled = 0;
    settings.reqs_per_event = 20;
    settings.backlog = 1024;
    settings.binding_protocol = negotiating_prot;
    settings.item_size_max = 1024 * 1024;  
}

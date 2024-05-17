static void stats_init(void) {
    stats.curr_items = stats.total_items = stats.curr_conns = stats.total_conns = stats.conn_structs = 0;
    stats.get_cmds = stats.set_cmds = stats.get_hits = stats.get_misses = stats.evictions = 0;
    stats.curr_bytes = stats.listen_disabled_num = 0;
    stats.accepting_conns = true;  

     
    process_started = time(0) - 2;
    stats_prefix_init();
}

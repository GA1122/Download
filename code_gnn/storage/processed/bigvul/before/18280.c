static rel_time_t realtime(const time_t exptime) {
     

    if (exptime == 0) return 0;  

    if (exptime > REALTIME_MAXDELTA) {
         
        if (exptime <= process_started)
            return (rel_time_t)1;
        return (rel_time_t)(exptime - process_started);
    } else {
        return (rel_time_t)(exptime + current_time);
    }
}

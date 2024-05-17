send_timer_proc(TimerClientData client_data, struct timeval *nowP)
{
    struct iperf_stream *sp = client_data.p;

     
    iperf_check_throttle(sp, nowP);
}

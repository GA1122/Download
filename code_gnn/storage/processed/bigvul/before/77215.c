append_port_stat(struct ofport *port, struct ovs_list *replies)
{
    struct ofputil_port_stats ops = { .port_no = port->pp.port_no };

    calc_duration(port->created, time_msec(),
                  &ops.duration_sec, &ops.duration_nsec);

     
    ofproto_port_get_stats(port, &ops.stats);

    ofputil_append_port_stat(replies, &ops);
}

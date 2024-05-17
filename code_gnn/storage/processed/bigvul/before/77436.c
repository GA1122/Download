update_port(struct ofproto *ofproto, const char *name)
{
    struct ofproto_port ofproto_port;
    struct ofputil_phy_port pp;
    struct netdev *netdev;
    struct ofport *port;
    int error = 0;

    COVERAGE_INC(ofproto_update_port);

     
    netdev = (!ofproto_port_query_by_name(ofproto, name, &ofproto_port)
              ? ofport_open(ofproto, &ofproto_port, &pp)
              : NULL);

    if (netdev) {
        port = ofproto_get_port(ofproto, ofproto_port.ofp_port);
        if (port && !strcmp(netdev_get_name(port->netdev), name)) {
            struct netdev *old_netdev = port->netdev;

             
            if (!ofport_equal(&port->pp, &pp)) {
                ofport_modified(port, &pp);
            }

            update_mtu(ofproto, port);

             
            port->netdev = netdev;
            port->change_seq = netdev_get_change_seq(netdev);

            if (port->ofproto->ofproto_class->port_modified) {
                port->ofproto->ofproto_class->port_modified(port);
            }

            netdev_close(old_netdev);
        } else {
             
            if (port) {
                ofport_remove(port);
            }
            ofport_remove_with_name(ofproto, name);
            error = ofport_install(ofproto, netdev, &pp);
        }
    } else {
         
        ofport_remove_with_name(ofproto, name);
    }
    ofproto_port_destroy(&ofproto_port);

    return error;
}

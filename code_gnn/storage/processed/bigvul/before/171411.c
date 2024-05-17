static int smart_socket_enqueue(asocket* s, apacket* p) {
 unsigned len;
#if ADB_HOST
 char* service = nullptr;
 char* serial = nullptr;
 TransportType type = kTransportAny;
#endif

    D("SS(%d): enqueue %d", s->id, p->len);

 if (s->pkt_first == 0) {
        s->pkt_first = p;
        s->pkt_last = p;
 } else {
 if ((s->pkt_first->len + p->len) > s->get_max_payload()) {
            D("SS(%d): overflow", s->id);
            put_apacket(p);
 goto fail;
 }

        memcpy(s->pkt_first->data + s->pkt_first->len, p->data, p->len);
        s->pkt_first->len += p->len;
        put_apacket(p);

        p = s->pkt_first;
 }

  
 if (p->len < 4) {
 return 0;
 }

    len = unhex(p->data, 4);
 if ((len < 1) || (len > MAX_PAYLOAD_V1)) {
        D("SS(%d): bad size (%d)", s->id, len);
 goto fail;
 }

    D("SS(%d): len is %d", s->id, len);
  
 if ((len + 4) > p->len) {
        D("SS(%d): waiting for %d more bytes", s->id, len + 4 - p->len);
 return 0;
 }

    p->data[len + 4] = 0;

    D("SS(%d): '%s'", s->id, (char*)(p->data + 4));

#if ADB_HOST
    service = (char*)p->data + 4;
 if (!strncmp(service, "host-serial:", strlen("host-serial:"))) {
 char* serial_end;
        service += strlen("host-serial:");

        serial_end = internal::skip_host_serial(service);
 if (serial_end) {
 *serial_end = 0;  
            serial = service;
            service = serial_end + 1;
 }
 } else if (!strncmp(service, "host-usb:", strlen("host-usb:"))) {
        type = kTransportUsb;
        service += strlen("host-usb:");
 } else if (!strncmp(service, "host-local:", strlen("host-local:"))) {
        type = kTransportLocal;
        service += strlen("host-local:");
 } else if (!strncmp(service, "host:", strlen("host:"))) {
        type = kTransportAny;
        service += strlen("host:");
 } else {
        service = nullptr;
 }

 if (service) {
        asocket* s2;

  
 if (handle_host_request(service, type, serial, s->peer->fd, s) == 0) {
  
            D("SS(%d): handled host service '%s'", s->id, service);
 goto fail;
 }
 if (!strncmp(service, "transport", strlen("transport"))) {
            D("SS(%d): okay transport", s->id);
            p->len = 0;
 return 0;
 }

  
        s2 = create_host_service_socket(service, serial);
 if (s2 == 0) {
            D("SS(%d): couldn't create host service '%s'", s->id, service);
 SendFail(s->peer->fd, "unknown host service");
 goto fail;
 }

  
 SendOkay(s->peer->fd);

        s->peer->ready = local_socket_ready;
        s->peer->shutdown = nullptr;
        s->peer->close = local_socket_close;
        s->peer->peer = s2;
        s2->peer = s->peer;
        s->peer = 0;
        D("SS(%d): okay", s->id);
        s->close(s);

  
        s2->ready(s2);
 return 0;
 }
#else  
 if (s->transport == nullptr) {
        std::string error_msg = "unknown failure";
        s->transport = acquire_one_transport(kTransportAny, nullptr, nullptr, &error_msg);
 if (s->transport == nullptr) {
 SendFail(s->peer->fd, error_msg);
 goto fail;
 }
 }
#endif

 if (!(s->transport) || (s->transport->connection_state == kCsOffline)) {
  
 SendFail(s->peer->fd, "device offline (x)");
 goto fail;
 }

  
    s->peer->ready = local_socket_ready_notify;
    s->peer->shutdown = nullptr;
    s->peer->close = local_socket_close_notify;
    s->peer->peer = 0;
  
    s->peer->transport = s->transport;

    connect_to_remote(s->peer, (char*)(p->data + 4));
    s->peer = 0;
    s->close(s);
 return 1;

fail:
  
    s->close(s);
 return -1;
}

void SetPortForAllAddrinfos(struct addrinfo* head, uint16 port) {
  DCHECK(head);
  for (struct addrinfo* ai = head; ai; ai = ai->ai_next) {
    uint16* port_field = GetPortFieldFromAddrinfo(ai);
    if (port_field)
      *port_field = htons(port);
  }
}

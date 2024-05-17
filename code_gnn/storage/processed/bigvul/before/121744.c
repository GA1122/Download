void UDPSocketLibevent::AllowBroadcast() {
  DCHECK(CalledOnValidThread());
  DCHECK(!is_connected());

  socket_options_ |= SOCKET_OPTION_BROADCAST;
}

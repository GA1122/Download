void UDPSocketWin::AllowAddressReuse() {
  DCHECK(CalledOnValidThread());
  DCHECK(!is_connected());

  socket_options_ |= SOCKET_OPTION_REUSE_ADDRESS;
}

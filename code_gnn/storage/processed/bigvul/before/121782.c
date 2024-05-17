void UDPSocketWin::Close() {
  DCHECK(CalledOnValidThread());

  if (!is_connected())
    return;

  read_callback_.Reset();
  recv_from_address_ = NULL;
  write_callback_.Reset();

  base::TimeTicks start_time = base::TimeTicks::Now();
  closesocket(socket_);
  UMA_HISTOGRAM_TIMES("Net.UDPSocketWinClose",
                      base::TimeTicks::Now() - start_time);
  socket_ = INVALID_SOCKET;
  addr_family_ = 0;

  core_->Detach();
  core_ = NULL;
}

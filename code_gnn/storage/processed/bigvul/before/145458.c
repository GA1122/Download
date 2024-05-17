void QuicTransportHost::CreateStream(
    std::unique_ptr<QuicStreamHost> stream_host) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

  P2PQuicStream* p2p_stream = quic_transport_->CreateStream();
  stream_host->Initialize(this, p2p_stream);
  stream_hosts_.insert(
      std::make_pair(stream_host.get(), std::move(stream_host)));
}

void QuicTransportHost::OnStream(P2PQuicStream* p2p_stream) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  DCHECK(p2p_stream);

  auto stream_proxy = std::make_unique<QuicStreamProxy>();
  auto stream_host = std::make_unique<QuicStreamHost>();
  stream_proxy->set_host(stream_host->AsWeakPtr());
  stream_host->set_proxy(stream_proxy->AsWeakPtr());

  stream_host->Initialize(this, p2p_stream);

  stream_hosts_.insert(
      std::make_pair(stream_host.get(), std::move(stream_host)));

  PostCrossThreadTask(*proxy_thread(), FROM_HERE,
                      CrossThreadBind(&QuicTransportProxy::OnStream, proxy_,
                                      WTF::Passed(std::move(stream_proxy))));
}

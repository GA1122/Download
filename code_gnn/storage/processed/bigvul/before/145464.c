QuicTransportHost::QuicTransportHost(
    base::WeakPtr<QuicTransportProxy> proxy,
    std::unique_ptr<P2PQuicTransportFactory> quic_transport_factory)
    : quic_transport_factory_(std::move(quic_transport_factory)),
      proxy_(std::move(proxy)) {
  DETACH_FROM_THREAD(thread_checker_);
  DCHECK(quic_transport_factory_);
  DCHECK(proxy_);
}

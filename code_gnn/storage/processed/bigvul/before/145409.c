P2PQuicTransportImpl::~P2PQuicTransportImpl() {
  packet_transport_->SetReceiveDelegate(nullptr);
}

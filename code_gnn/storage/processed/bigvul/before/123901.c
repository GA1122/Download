void RenderViewImpl::OnReleaseDisambiguationPopupDIB(
    TransportDIB::Handle dib_handle) {
  TransportDIB* dib = TransportDIB::CreateWithHandle(dib_handle);
  RenderProcess::current()->ReleaseTransportDIB(dib);
}

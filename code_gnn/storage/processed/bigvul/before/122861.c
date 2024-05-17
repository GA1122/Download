void RenderProcessHostImpl::ClearTransportDIBCache() {
#if defined(USE_X11)
  std::map<TransportDIB::Id, TransportDIB*>::const_iterator dib =
      cached_dibs_.begin();
  for (; dib != cached_dibs_.end(); ++dib)
    dib->second->Detach();
#else
  STLDeleteContainerPairSecondPointers(
      cached_dibs_.begin(), cached_dibs_.end());
#endif
  cached_dibs_.clear();
}

bool DocumentInit::IsHostedInReservedIPRange() const {
  if (DocumentLoader* loader = MasterDocumentLoader()) {
    if (!loader->GetResponse().RemoteIPAddress().IsEmpty()) {
      return network_utils::IsReservedIPAddress(
          loader->GetResponse().RemoteIPAddress());
    }
  }
  return false;
}

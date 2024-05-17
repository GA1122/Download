bool DocumentInit::IsHostedInReservedIPRange() const {
  if (DocumentLoader* loader = MasterDocumentLoader()) {
    if (!loader->GetResponse().RemoteIPAddress().IsEmpty()) {
      return NetworkUtils::IsReservedIPAddress(
          loader->GetResponse().RemoteIPAddress());
    }
  }
  return false;
}

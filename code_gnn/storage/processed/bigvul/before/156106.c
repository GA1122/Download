LinkImport* HTMLLinkElement::GetLinkImport() const {
  if (!link_ || link_->GetType() != LinkResource::kImport)
    return nullptr;
  return static_cast<LinkImport*>(link_.Get());
}

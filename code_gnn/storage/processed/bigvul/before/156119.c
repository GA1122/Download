LinkResource* HTMLLinkElement::LinkResourceToProcess() {
  if (!ShouldLoadLink()) {
    if (GetLinkStyle() && GetLinkStyle()->HasSheet())
      return GetLinkStyle();
    return nullptr;
  }

  if (!link_) {
    if (rel_attribute_.IsImport() &&
        RuntimeEnabledFeatures::HTMLImportsEnabled()) {
      link_ = LinkImport::Create(this);
    } else if (rel_attribute_.IsManifest()) {
      link_ = LinkManifest::Create(this);
    } else {
      LinkStyle* link = LinkStyle::Create(this);
      if (FastHasAttribute(disabledAttr)) {
        UseCounter::Count(GetDocument(), WebFeature::kHTMLLinkElementDisabled);
        link->SetDisabledState(true);
      }
      link_ = link;
    }
  }

  return link_.Get();
}

bool CSSStyleSheet::CanBeActivated(
    const String& current_preferrable_name) const {
  if (disabled())
    return false;

  if (owner_node_ && owner_node_->IsInShadowTree()) {
    if (IsHTMLStyleElement(owner_node_) || IsSVGStyleElement(owner_node_))
      return true;
    if (IsHTMLLinkElement(owner_node_) &&
        ToHTMLLinkElement(owner_node_)->IsImport())
      return !IsAlternate();
  }

  if (!owner_node_ ||
      owner_node_->getNodeType() == Node::kProcessingInstructionNode ||
      !IsHTMLLinkElement(owner_node_) ||
      !ToHTMLLinkElement(owner_node_)->IsEnabledViaScript()) {
    if (!title_.IsEmpty() && title_ != current_preferrable_name)
      return false;
  }

  if (IsAlternate() && title_.IsEmpty())
    return false;

  return true;
}

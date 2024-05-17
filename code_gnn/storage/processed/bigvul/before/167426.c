void HTMLStyleElement::ParseAttribute(
    const AttributeModificationParams& params) {
  if (params.name == titleAttr && sheet_ && IsInDocumentTree()) {
    sheet_->SetTitle(params.new_value);
  } else if (params.name == mediaAttr && isConnected() &&
             GetDocument().IsActive() && sheet_) {
    sheet_->SetMediaQueries(MediaQuerySet::Create(params.new_value));
    GetDocument().GetStyleEngine().MediaQueriesChangedInScope(GetTreeScope());
  } else if (params.name == typeAttr) {
    HTMLElement::ParseAttribute(params);
    StyleElement::ChildrenChanged(*this);
  } else {
    HTMLElement::ParseAttribute(params);
  }
}

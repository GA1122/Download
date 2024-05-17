void ScrollAnchor::ClearSelf() {
  LayoutObject* anchor_object = anchor_object_;
  anchor_object_ = nullptr;
  saved_selector_ = String();

  if (anchor_object)
    anchor_object->MaybeClearIsScrollAnchorObject();
}

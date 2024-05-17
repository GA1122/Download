void RemoteFrame::SetCcLayer(cc::Layer* cc_layer,
                             bool prevent_contents_opaque_changes,
                             bool is_surface_layer) {
  DCHECK(Owner());

  if (cc_layer_)
    GraphicsLayer::UnregisterContentsLayer(cc_layer_);
  cc_layer_ = cc_layer;
  prevent_contents_opaque_changes_ = prevent_contents_opaque_changes;
  is_surface_layer_ = is_surface_layer;
  if (cc_layer_) {
    GraphicsLayer::RegisterContentsLayer(cc_layer_);
    PointerEventsChanged();
  }

  ToHTMLFrameOwnerElement(Owner())->SetNeedsCompositingUpdate();
}

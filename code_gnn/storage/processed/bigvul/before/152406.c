void RenderFrameImpl::MarkWebAXObjectDirty(const blink::WebAXObject& obj,
                                           bool subtree) {
  if (render_accessibility_)
    render_accessibility_->MarkWebAXObjectDirty(obj, subtree);
}

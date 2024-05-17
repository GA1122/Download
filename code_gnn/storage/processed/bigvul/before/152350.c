blink::WebComputedAXTree* RenderFrameImpl::GetOrCreateWebComputedAXTree() {
  if (!computed_ax_tree_)
    computed_ax_tree_ = std::make_unique<AomContentAxTree>(this);
  return computed_ax_tree_.get();
}

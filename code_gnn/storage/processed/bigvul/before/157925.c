void RenderViewImpl::FocusedNodeChanged(const WebNode& fromNode,
                                        const WebNode& toNode) {
  RenderFrameImpl* previous_frame = nullptr;
  if (!fromNode.IsNull())
    previous_frame =
        RenderFrameImpl::FromWebFrame(fromNode.GetDocument().GetFrame());
  RenderFrameImpl* new_frame = nullptr;
  if (!toNode.IsNull())
    new_frame = RenderFrameImpl::FromWebFrame(toNode.GetDocument().GetFrame());

  if (previous_frame && previous_frame != new_frame)
    previous_frame->FocusedNodeChanged(WebNode());
  if (new_frame)
    new_frame->FocusedNodeChanged(toNode);

  if (main_render_frame_)
    main_render_frame_->FocusedNodeChangedForAccessibility(toNode);
}

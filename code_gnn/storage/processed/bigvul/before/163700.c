void WebContentsImpl::FocusOuterAttachmentFrameChain() {
  WebContentsImpl* outer_contents = GetOuterWebContents();
  if (!outer_contents)
    return;

  FrameTreeNode* outer_node =
      FrameTreeNode::GloballyFindByID(GetOuterDelegateFrameTreeNodeId());
  outer_contents->frame_tree_.SetFocusedFrame(outer_node, nullptr);

  if (GetRenderManager()->GetProxyToOuterDelegate())
    GetRenderManager()->GetProxyToOuterDelegate()->SetFocusedFrame();

  outer_contents->FocusOuterAttachmentFrameChain();
}

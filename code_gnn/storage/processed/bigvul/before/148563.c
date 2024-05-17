void WebContentsImpl::SetOpenerForNewContents(FrameTreeNode* opener,
                                              bool opener_suppressed) {
  if (opener) {
    FrameTreeNode* new_root = GetFrameTree()->root();

    new_root->SetOriginalOpener(opener->frame_tree()->root());

    if (!opener_suppressed) {
      new_root->SetOpener(opener);
      created_with_opener_ = true;
    }
  }
}

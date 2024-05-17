bool HasMatchingProcess(FrameTree* tree, int render_process_id) {
  for (FrameTreeNode* node : tree->Nodes()) {
    if (node->current_frame_host()->GetProcess()->GetID() == render_process_id)
      return true;
  }
  return false;
}

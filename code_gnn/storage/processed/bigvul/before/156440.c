void DispatchToAgents(int frame_tree_node_id,
                      void (Handler::*method)(MethodArgs...),
                      Args&&... args) {
  FrameTreeNode* ftn = FrameTreeNode::GloballyFindByID(frame_tree_node_id);
  if (ftn)
    DispatchToAgents(ftn, method, std::forward<Args>(args)...);
}

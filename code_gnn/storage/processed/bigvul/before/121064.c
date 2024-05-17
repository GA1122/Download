void AddDependenciesToGraphNode(
    internal::GraphNode* node,
    const NodeVector::ContainerType& dependencies) {
  for (NodeVector::ContainerType::const_iterator it = dependencies.begin();
       it != dependencies.end(); ++it) {
    internal::GraphNode* dependency = *it;

    node->add_dependency();
    dependency->add_dependent(node);
  }
}

  const cc::TransformNode& GetTransformNode(const cc::Layer* layer) {
    return *GetPropertyTrees().transform_tree.Node(
        layer->transform_tree_index());
  }

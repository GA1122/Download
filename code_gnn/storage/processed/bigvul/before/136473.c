int PropertyTreeManager::EnsureCompositorTransformNode(
    const TransformPaintPropertyNode* transform_node) {
  DCHECK(transform_node);
  if (!transform_node)
    return kSecondaryRootNodeId;

  auto it = transform_node_map_.find(transform_node);
  if (it != transform_node_map_.end())
    return it->value;

  int parent_id = EnsureCompositorTransformNode(transform_node->Parent());
  int id = GetTransformTree().Insert(cc::TransformNode(), parent_id);

  cc::TransformNode& compositor_node = *GetTransformTree().Node(id);
  compositor_node.source_node_id = parent_id;

  FloatPoint3D origin = transform_node->Origin();
  compositor_node.pre_local.matrix().setTranslate(-origin.X(), -origin.Y(),
                                                  -origin.Z());
  compositor_node.local.matrix() =
      TransformationMatrix::ToSkMatrix44(transform_node->Matrix());
  compositor_node.post_local.matrix().setTranslate(origin.X(), origin.Y(),
                                                   origin.Z());
  compositor_node.needs_local_transform_update = true;
  compositor_node.flattens_inherited_transform =
      transform_node->FlattensInheritedTransform();
  compositor_node.sorting_context_id = transform_node->RenderingContextId();

  CompositorElementId compositor_element_id =
      transform_node->GetCompositorElementId();
  if (compositor_element_id) {
    property_trees_.element_id_to_transform_node_index[compositor_element_id] =
        id;
  }

  if (auto* scroll_node = transform_node->ScrollNode()) {
    auto scroll_offset_size = transform_node->Matrix().To2DTranslation();
    auto scroll_offset = gfx::ScrollOffset(-scroll_offset_size.Width(),
                                           -scroll_offset_size.Height());
    DCHECK(compositor_node.local.IsIdentityOr2DTranslation());
    compositor_node.scroll_offset = scroll_offset;
    compositor_node.local.MakeIdentity();
    compositor_node.scrolls = true;

    CreateCompositorScrollNode(scroll_node, compositor_node);
  }

  auto result = transform_node_map_.Set(transform_node, id);
  DCHECK(result.is_new_entry);
  GetTransformTree().set_needs_update(true);

  return id;
}

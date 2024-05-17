void PaintArtifactCompositor::Update(
    const PaintArtifact& paint_artifact,
    CompositorElementIdSet& composited_element_ids) {
  DCHECK(root_layer_);

  cc::LayerTreeHost* host = root_layer_->layer_tree_host();
  if (!host)
    return;

  if (extra_data_for_testing_enabled_)
    extra_data_for_testing_.reset(new ExtraDataForTesting);

  RemoveChildLayers();
  root_layer_->set_property_tree_sequence_number(
      g_s_property_tree_sequence_number);

  PropertyTreeManager property_tree_manager(*this, *host->property_trees(),
                                            root_layer_.get(),
                                            g_s_property_tree_sequence_number);
  Vector<PendingLayer, 0> pending_layers;
  CollectPendingLayers(paint_artifact, pending_layers);

  Vector<std::unique_ptr<ContentLayerClientImpl>> new_content_layer_clients;
  new_content_layer_clients.ReserveCapacity(pending_layers.size());
  Vector<scoped_refptr<cc::Layer>> new_scroll_hit_test_layers;

  for (auto& entry : synthesized_clip_cache_)
    entry.in_use = false;

  for (auto& pending_layer : pending_layers) {
    const auto& property_state = pending_layer.property_tree_state;
    const auto* transform = property_state.Transform();
    const auto* clip = property_state.Clip();

    if (clip->LocalTransformSpace() == transform) {
      pending_layer.bounds.Intersect(clip->ClipRect().Rect());
    } else if (const auto* scroll = transform->ScrollNode()) {
      pending_layer.bounds.Intersect(FloatRect(scroll->ContentsRect()));
    }

    gfx::Vector2dF layer_offset;
    scoped_refptr<cc::Layer> layer = CompositedLayerForPendingLayer(
        paint_artifact, pending_layer, layer_offset, new_content_layer_clients,
        new_scroll_hit_test_layers);

    int transform_id =
        property_tree_manager.EnsureCompositorTransformNode(transform);
    int clip_id = property_tree_manager.EnsureCompositorClipNode(clip);
    int effect_id = property_tree_manager.SwitchToEffectNodeWithSynthesizedClip(
        *property_state.Effect(), *clip);
    const auto& scroll_translation =
        ScrollTranslationForPendingLayer(paint_artifact, pending_layer);
    int scroll_id =
        property_tree_manager.EnsureCompositorScrollNode(&scroll_translation);

    layer->set_offset_to_transform_parent(layer_offset);

    CompositorElementId element_id =
        layer->scrollable()
            ? layer->element_id()
            : property_state.GetCompositorElementId(composited_element_ids);
    if (element_id) {
      layer->SetElementId(element_id);
      composited_element_ids.insert(element_id);
    }

    root_layer_->AddChild(layer);
    if (element_id) {
      host->RegisterElement(element_id, cc::ElementListType::ACTIVE,
                            layer.get());
    }

    layer->set_property_tree_sequence_number(g_s_property_tree_sequence_number);
    layer->SetTransformTreeIndex(transform_id);
    layer->SetScrollTreeIndex(scroll_id);
    layer->SetClipTreeIndex(clip_id);
    layer->SetEffectTreeIndex(effect_id);
    bool backface_hidden =
        IsBackfaceHidden(pending_layer.property_tree_state.Transform());
    layer->SetDoubleSided(!backface_hidden);
    layer->SetShouldCheckBackfaceVisibility(backface_hidden);
  }
  property_tree_manager.Finalize();
  content_layer_clients_.swap(new_content_layer_clients);
  scroll_hit_test_layers_.swap(new_scroll_hit_test_layers);

  synthesized_clip_cache_.erase(
      std::remove_if(synthesized_clip_cache_.begin(),
                     synthesized_clip_cache_.end(),
                     [](const auto& entry) { return !entry.in_use; }),
      synthesized_clip_cache_.end());
  if (extra_data_for_testing_enabled_) {
    for (const auto& entry : synthesized_clip_cache_) {
      extra_data_for_testing_->synthesized_clip_layers.push_back(
          entry.synthesized_clip->GetLayer());
    }
  }

  host->property_trees()->sequence_number = g_s_property_tree_sequence_number;
  host->property_trees()->needs_rebuild = false;
  host->property_trees()->ResetCachedData();

  g_s_property_tree_sequence_number++;

#if DCHECK_IS_ON()
  if (VLOG_IS_ON(2)) {
    static String s_previous_output;
    LayerTreeFlags flags = VLOG_IS_ON(3) ? 0xffffffff : 0;
    String new_output = LayersAsJSON(flags)->ToPrettyJSONString();
    if (new_output != s_previous_output) {
      LOG(ERROR) << "PaintArtifactCompositor::Update() done\n"
                 << "Composited layers:\n"
                 << new_output.Utf8().data();
      s_previous_output = new_output;
    }
  }
#endif
}

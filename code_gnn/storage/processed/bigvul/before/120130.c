void Layer::PushPropertiesTo(LayerImpl* layer) {
  DCHECK(layer_tree_host_);

  bool use_paint_properties = paint_properties_.source_frame_number ==
                              layer_tree_host_->source_frame_number();

  layer->SetAnchorPoint(anchor_point_);
  layer->SetAnchorPointZ(anchor_point_z_);
  layer->SetBackgroundColor(background_color_);
  layer->SetBounds(use_paint_properties ? paint_properties_.bounds
                                        : bounds_);
  layer->SetContentBounds(content_bounds());
  layer->SetContentsScale(contents_scale_x(), contents_scale_y());

  bool is_tracing;
  TRACE_EVENT_CATEGORY_GROUP_ENABLED(TRACE_DISABLED_BY_DEFAULT("cc.debug"),
                                     &is_tracing);
  if (is_tracing)
    layer->SetDebugInfo(TakeDebugInfo());

  layer->SetDoubleSided(double_sided_);
  layer->SetDrawCheckerboardForMissingTiles(
      draw_checkerboard_for_missing_tiles_);
  layer->SetForceRenderSurface(force_render_surface_);
  layer->SetDrawsContent(DrawsContent());
  layer->SetHideLayerAndSubtree(hide_layer_and_subtree_);
  if (!layer->FilterIsAnimatingOnImplOnly() && !FilterIsAnimating())
    layer->SetFilters(filters_);
  DCHECK(!(FilterIsAnimating() && layer->FilterIsAnimatingOnImplOnly()));
  layer->SetBackgroundFilters(background_filters());
  layer->SetMasksToBounds(masks_to_bounds_);
  layer->SetShouldScrollOnMainThread(should_scroll_on_main_thread_);
  layer->SetHaveWheelEventHandlers(have_wheel_event_handlers_);
  layer->SetNonFastScrollableRegion(non_fast_scrollable_region_);
  layer->SetTouchEventHandlerRegion(touch_event_handler_region_);
  layer->SetContentsOpaque(contents_opaque_);
  if (!layer->OpacityIsAnimatingOnImplOnly() && !OpacityIsAnimating())
    layer->SetOpacity(opacity_);
  DCHECK(!(OpacityIsAnimating() && layer->OpacityIsAnimatingOnImplOnly()));
  layer->SetBlendMode(blend_mode_);
  layer->SetIsRootForIsolatedGroup(is_root_for_isolated_group_);
  layer->SetPosition(position_);
  layer->SetIsContainerForFixedPositionLayers(
      IsContainerForFixedPositionLayers());
  layer->SetPositionConstraint(position_constraint_);
  layer->SetShouldFlattenTransform(should_flatten_transform_);
  layer->SetIs3dSorted(is_3d_sorted_);
  layer->SetUseParentBackfaceVisibility(use_parent_backface_visibility_);
  if (!layer->TransformIsAnimatingOnImplOnly() && !TransformIsAnimating())
    layer->SetTransform(transform_);
  DCHECK(!(TransformIsAnimating() && layer->TransformIsAnimatingOnImplOnly()));

  layer->SetScrollClipLayer(scroll_clip_layer_id_);
  layer->set_user_scrollable_horizontal(user_scrollable_horizontal_);
  layer->set_user_scrollable_vertical(user_scrollable_vertical_);

  LayerImpl* scroll_parent = NULL;
  if (scroll_parent_) {
    scroll_parent = layer->layer_tree_impl()->LayerById(scroll_parent_->id());
    DCHECK(scroll_parent);
  }

  layer->SetScrollParent(scroll_parent);
  if (scroll_children_) {
    std::set<LayerImpl*>* scroll_children = new std::set<LayerImpl*>;
    for (std::set<Layer*>::iterator it = scroll_children_->begin();
         it != scroll_children_->end();
         ++it) {
      DCHECK_EQ((*it)->scroll_parent(), this);
      LayerImpl* scroll_child =
          layer->layer_tree_impl()->LayerById((*it)->id());
      DCHECK(scroll_child);
      scroll_children->insert(scroll_child);
    }
    layer->SetScrollChildren(scroll_children);
  } else {
    layer->SetScrollChildren(NULL);
  }

  LayerImpl* clip_parent = NULL;
  if (clip_parent_) {
    clip_parent =
        layer->layer_tree_impl()->LayerById(clip_parent_->id());
    DCHECK(clip_parent);
  }

  layer->SetClipParent(clip_parent);
  if (clip_children_) {
    std::set<LayerImpl*>* clip_children = new std::set<LayerImpl*>;
    for (std::set<Layer*>::iterator it = clip_children_->begin();
        it != clip_children_->end(); ++it) {
      DCHECK_EQ((*it)->clip_parent(), this);
      LayerImpl* clip_child = layer->layer_tree_impl()->LayerById((*it)->id());
      DCHECK(clip_child);
      clip_children->insert(clip_child);
    }
    layer->SetClipChildren(clip_children);
  } else {
    layer->SetClipChildren(NULL);
  }

  if (layer->layer_tree_impl()->settings().impl_side_painting) {
    layer->SetScrollOffset(scroll_offset_);
  } else {
    layer->SetScrollOffsetAndDelta(
        scroll_offset_, layer->ScrollDelta() - layer->sent_scroll_delta());
    layer->SetSentScrollDelta(gfx::Vector2d());
  }

  ScopedPtrVector<CopyOutputRequest> main_thread_copy_requests;
  for (ScopedPtrVector<CopyOutputRequest>::iterator it = copy_requests_.begin();
       it != copy_requests_.end();
       ++it) {
    scoped_refptr<base::SingleThreadTaskRunner> main_thread_task_runner =
        layer_tree_host()->proxy()->MainThreadTaskRunner();
    scoped_ptr<CopyOutputRequest> original_request = copy_requests_.take(it);
    const CopyOutputRequest& original_request_ref = *original_request;
    scoped_ptr<CopyOutputRequest> main_thread_request =
        CopyOutputRequest::CreateRelayRequest(
            original_request_ref,
            base::Bind(&PostCopyCallbackToMainThread,
                       main_thread_task_runner,
                       base::Passed(&original_request)));
    main_thread_copy_requests.push_back(main_thread_request.Pass());
  }
  copy_requests_.clear();
  layer->PassCopyRequests(&main_thread_copy_requests);

  update_rect_.Union(layer->update_rect());
  layer->SetUpdateRect(update_rect_);

  layer->SetStackingOrderChanged(stacking_order_changed_);

  layer_animation_controller_->PushAnimationUpdatesTo(
      layer->layer_animation_controller());

  stacking_order_changed_ = false;
  update_rect_ = gfx::RectF();

  needs_push_properties_ = false;
  num_dependents_need_push_properties_ = 0;
}

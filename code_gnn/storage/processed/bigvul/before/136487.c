  GraphicsLayerTest() {
    clip_layer_ = std::make_unique<FakeGraphicsLayer>(client_);
    scroll_elasticity_layer_ = std::make_unique<FakeGraphicsLayer>(client_);
    page_scale_layer_ = std::make_unique<FakeGraphicsLayer>(client_);
    graphics_layer_ = std::make_unique<FakeGraphicsLayer>(client_);
    graphics_layer_->SetDrawsContent(true);
    clip_layer_->AddChild(scroll_elasticity_layer_.get());
    scroll_elasticity_layer_->AddChild(page_scale_layer_.get());
    page_scale_layer_->AddChild(graphics_layer_.get());
    graphics_layer_->CcLayer()->SetScrollable(clip_layer_->CcLayer()->bounds());
    cc_layer_ = graphics_layer_->CcLayer();
    layer_tree_view_ = std::make_unique<WebLayerTreeViewImplForTesting>();
    DCHECK(layer_tree_view_);
    layer_tree_view_->SetRootLayer(clip_layer_->CcLayer());
    WebLayerTreeView::ViewportLayers viewport_layers;
    viewport_layers.overscroll_elasticity = scroll_elasticity_layer_->CcLayer();
    viewport_layers.page_scale = page_scale_layer_->CcLayer();
    viewport_layers.inner_viewport_container = clip_layer_->CcLayer();
    viewport_layers.inner_viewport_scroll = graphics_layer_->CcLayer();
    layer_tree_view_->RegisterViewportLayers(viewport_layers);
    layer_tree_view_->SetViewportSize(WebSize(1, 1));

      graphics_layer_->SetLayerState(
          PropertyTreeState(PropertyTreeState::Root()), IntPoint());
  }

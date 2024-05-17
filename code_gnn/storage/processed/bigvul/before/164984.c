void HTMLCanvasElement::CreateLayer() {
  DCHECK(!surface_layer_bridge_);
  LocalFrame* frame = GetDocument().GetFrame();
  WebLayerTreeView* layer_tree_view = nullptr;
  if (frame) {
    layer_tree_view =
        frame->GetPage()->GetChromeClient().GetWebLayerTreeView(frame);
    surface_layer_bridge_ = std::make_unique<::blink::SurfaceLayerBridge>(
        layer_tree_view, this, base::DoNothing());
    surface_layer_bridge_->CreateSolidColorLayer();
  }
}

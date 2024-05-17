void HTMLCanvasElement::DisableAcceleration(
    std::unique_ptr<Canvas2DLayerBridge>
        unaccelerated_bridge_used_for_testing) {
  if (base::FeatureList::IsEnabled(features::kAlwaysAccelerateCanvas)) {
    NOTREACHED();
  }
  std::unique_ptr<Canvas2DLayerBridge> bridge;
  if (unaccelerated_bridge_used_for_testing)
    bridge = std::move(unaccelerated_bridge_used_for_testing);
  else
    bridge = CreateUnaccelerated2dBuffer();

  if (bridge && canvas2d_bridge_)
    ReplaceExisting2dLayerBridge(std::move(bridge));

  DidDraw();
  SetNeedsCompositingUpdate();
}

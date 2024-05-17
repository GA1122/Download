HTMLCanvasElement::~HTMLCanvasElement() {
  if (surface_layer_bridge_ && surface_layer_bridge_->GetCcLayer())
    GraphicsLayer::UnregisterContentsLayer(surface_layer_bridge_->GetCcLayer());
  v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(
      -externally_allocated_memory_);
}

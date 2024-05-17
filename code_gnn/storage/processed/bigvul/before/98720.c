Graphics2DDeviceContext* WebPluginDelegatePepper::GetGraphicsContext(
    NPDeviceContext2D* context) {
  return graphic2d_contexts_.Lookup(
      reinterpret_cast<intptr_t>(context->reserved));
}

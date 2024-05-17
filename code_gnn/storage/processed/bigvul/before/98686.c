NPError WebPluginDelegatePepper::Device2DDestroyContext(
    NPDeviceContext2D* context) {

  if (!context || !graphic2d_contexts_.Lookup(
      reinterpret_cast<intptr_t>(context->reserved))) {
    return NPERR_INVALID_PARAM;
  }
  graphic2d_contexts_.Remove(reinterpret_cast<intptr_t>(context->reserved));
  memset(context, 0, sizeof(NPDeviceContext2D));
  return NPERR_NO_ERROR;
}

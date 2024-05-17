bool WebGLRenderingContextBase::IsXRDeviceCompatible(
    const XRDevice* xr_device) {
  return xr_device == compatible_xr_device_;
}

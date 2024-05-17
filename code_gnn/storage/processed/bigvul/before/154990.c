void WebGLRenderingContextBase::TextureUnitState::Trace(
    blink::Visitor* visitor) {
  visitor->Trace(texture2d_binding_);
  visitor->Trace(texture_cube_map_binding_);
  visitor->Trace(texture3d_binding_);
  visitor->Trace(texture2d_array_binding_);
  visitor->Trace(texture_video_image_binding_);
}

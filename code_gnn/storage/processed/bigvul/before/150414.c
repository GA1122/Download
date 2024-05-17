void ClientControlledShellSurface::UpdateFrameWidth() {
  int width = -1;
  if (shadow_bounds_) {
    float device_scale_factor =
        GetWidget()->GetNativeWindow()->layer()->device_scale_factor();
    float dsf_to_default_dsf = device_scale_factor / scale_;
    width = gfx::ToRoundedInt(shadow_bounds_->width() * dsf_to_default_dsf);
  }
  static_cast<ash::HeaderView*>(GetFrameView()->GetHeaderView())
      ->SetWidthInPixels(width);
}

void ClientControlledShellSurface::UpdateCaptionButtonModel() {
  auto model = std::make_unique<CaptionButtonModel>(frame_visible_button_mask_,
                                                    frame_enabled_button_mask_);
  if (wide_frame_)
    wide_frame_->SetCaptionButtonModel(std::move(model));
  else
    GetFrameView()->SetCaptionButtonModel(std::move(model));
}

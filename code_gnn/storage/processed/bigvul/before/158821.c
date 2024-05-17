gfx::Size Browser::EnterPictureInPicture(const viz::SurfaceId& surface_id,
                                         const gfx::Size& natural_size) {
  return PictureInPictureWindowManager::GetInstance()->EnterPictureInPicture(
      tab_strip_model_->GetActiveWebContents(), surface_id, natural_size);
}

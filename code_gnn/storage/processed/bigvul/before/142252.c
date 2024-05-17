 void ManualFillingControllerImpl::GetFavicon(
    int desired_size_in_pixel,
    base::OnceCallback<void(const gfx::Image&)> icon_callback) {
  DCHECK(pwd_controller_);
  pwd_controller_->GetFavicon(desired_size_in_pixel, std::move(icon_callback));
}

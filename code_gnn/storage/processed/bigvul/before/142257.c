void ManualFillingControllerImpl::OnFillingTriggered(
    bool is_password,
    const base::string16& text_to_fill) {
  DCHECK(pwd_controller_);
  pwd_controller_->OnFillingTriggered(is_password, text_to_fill);
}

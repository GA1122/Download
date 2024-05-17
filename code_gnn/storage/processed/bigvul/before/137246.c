void Textfield::OnBeforeUserAction() {
  DCHECK(!performing_user_action_);
  performing_user_action_ = true;
  if (controller_)
    controller_->OnBeforeUserAction(this);
}

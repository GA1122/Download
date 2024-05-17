 int GlobalConfirmInfoBar::DelegateProxy::GetButtons() const {
   return global_info_bar_ ? global_info_bar_->delegate_->GetButtons()
                           : 0;
}

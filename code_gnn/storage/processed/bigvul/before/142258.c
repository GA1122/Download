void ManualFillingControllerImpl::OnOptionSelected(
    const base::string16& selected_option) const {
  DCHECK(pwd_controller_);
  pwd_controller_->OnOptionSelected(selected_option);
 }

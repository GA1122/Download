void PasswordAccessoryControllerImpl::OnFillingTriggered(
    bool is_password,
    const base::string16& text_to_fill) {
  password_manager::ContentPasswordManagerDriverFactory* factory =
      password_manager::ContentPasswordManagerDriverFactory::FromWebContents(
          web_contents_);
  DCHECK(factory);
  password_manager::ContentPasswordManagerDriver* driver =
      factory->GetDriverForFrame(web_contents_->GetMainFrame());
  if (!driver) {
    return;
  }   
  driver->FillIntoFocusedField(
      is_password, text_to_fill,
      base::BindOnce(&PasswordAccessoryControllerImpl::OnFilledIntoFocusedField,
                      base::AsWeakPtr<PasswordAccessoryControllerImpl>(this)));
 }

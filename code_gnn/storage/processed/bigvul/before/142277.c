void ChromePasswordManagerClient::GeneratePassword() {
  password_manager::ContentPasswordManagerDriver* driver =
      driver_factory_->GetDriverForFrame(web_contents()->GetFocusedFrame());
  driver->GeneratePassword(base::BindOnce(
      &ChromePasswordManagerClient::ShowManualPasswordGenerationPopup,
      base::Unretained(this), base::AsWeakPtr(driver)));
}

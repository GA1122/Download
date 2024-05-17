void ChromePasswordManagerClient::ShowManualPasswordGenerationPopup(
    base::WeakPtr<password_manager::ContentPasswordManagerDriver> driver,
    const base::Optional<
        autofill::password_generation::PasswordGenerationUIData>& ui_data) {
  if (!ui_data || !driver)
    return;
  if (!password_manager::bad_message::CheckChildProcessSecurityPolicy(
          driver->render_frame_host(), ui_data->password_form,
          BadMessageReason::
              CPMD_BAD_ORIGIN_SHOW_MANUAL_PASSWORD_GENERATION_POPUP))
    return;
  ShowPasswordGenerationPopup(driver.get(), *ui_data,
                              true  );
}

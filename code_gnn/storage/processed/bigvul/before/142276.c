void ChromePasswordManagerClient::FocusedInputChanged(bool is_fillable,
                                                      bool is_password_field) {
#if defined(OS_ANDROID)
  if (PasswordAccessoryController::AllowedForWebContents(web_contents())) {
    PasswordAccessoryController::GetOrCreate(web_contents())
        ->RefreshSuggestionsForField(
            password_manager_driver_bindings_.GetCurrentTargetFrame()
                ->GetLastCommittedOrigin(),
            is_fillable, is_password_field);
  }
#endif   
}

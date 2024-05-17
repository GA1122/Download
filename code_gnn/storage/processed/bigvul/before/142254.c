ManualFillingControllerImpl::ManualFillingControllerImpl(
    content::WebContents* web_contents)
    : web_contents_(web_contents),
      view_(ManualFillingViewInterface::Create(this)),
      weak_factory_(this) {
  if (PasswordAccessoryController::AllowedForWebContents(web_contents)) {
    pwd_controller_ =
        PasswordAccessoryController::GetOrCreate(web_contents)->AsWeakPtr();
    DCHECK(pwd_controller_);
  }
}

PasswordAccessoryController* PasswordAccessoryController::GetOrCreate(
    content::WebContents* web_contents) {
  DCHECK(PasswordAccessoryController::AllowedForWebContents(web_contents));

  PasswordAccessoryControllerImpl::CreateForWebContents(web_contents);
  return PasswordAccessoryControllerImpl::FromWebContents(web_contents);
}

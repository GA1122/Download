PasswordAccessoryController* PasswordAccessoryController::GetIfExisting(
    content::WebContents* web_contents) {
   return PasswordAccessoryControllerImpl::FromWebContents(web_contents);
 }

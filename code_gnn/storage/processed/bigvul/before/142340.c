bool PasswordAccessoryController::AllowedForWebContents(
    content::WebContents* web_contents) {
  DCHECK(web_contents) << "Need valid WebContents to attach controller to!";
  if (vr::VrTabHelper::IsInVr(web_contents)) {
    return false;   
  }
  return base::FeatureList::IsEnabled(
             password_manager::features::kPasswordsKeyboardAccessory) ||
         base::FeatureList::IsEnabled(features::kExperimentalUi);
}

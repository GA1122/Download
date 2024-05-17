void TabHelper::CreateHostedAppFromWebContents() {
  DCHECK(CanCreateBookmarkApp());
  if (pending_web_app_action_ != NONE)
    return;

  GetApplicationInfo(CREATE_HOSTED_APP);
}

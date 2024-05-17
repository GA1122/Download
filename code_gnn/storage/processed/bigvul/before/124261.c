void PermissionsRequestFunction::InstallUIProceed() {
  PermissionsUpdater perms_updater(profile());
  perms_updater.AddPermissions(GetExtension(), requested_permissions_.get());

  results_ = Request::Results::Create(true);
  SendResponse(true);

  Release();   
 }

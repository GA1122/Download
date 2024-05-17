void PageCaptureSaveAsMHTMLFunction::ResolvePermissionRequest(
    const PermissionIDSet& allowed_permissions) {
  if (allowed_permissions.ContainsID(APIPermission::kPageCapture)) {
    base::PostTaskWithTraits(
        FROM_HERE, kCreateTemporaryFileTaskTraits,
        base::BindOnce(&PageCaptureSaveAsMHTMLFunction::CreateTemporaryFile,
                       this));
  } else {
    ReturnFailure(kUserDenied);
  }
}

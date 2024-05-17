    CreateWebViewPermissionHelperDelegate(
        WebViewPermissionHelper* web_view_permission_helper) const {
  return new WebViewPermissionHelperDelegate(web_view_permission_helper);
}

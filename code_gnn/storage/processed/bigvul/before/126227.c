void Browser::RequestMediaAccessPermission(
    content::WebContents* web_contents,
    const content::MediaStreamRequest* request,
    const content::MediaResponseCallback& callback) {
  RequestMediaAccessPermissionHelper(web_contents, request, callback);
}

void Browser::RequestMediaAccessPermission(
    content::WebContents* web_contents,
    const content::MediaStreamRequest& request,
    const content::MediaResponseCallback& callback) {
  ::RequestMediaAccessPermission(web_contents, profile_, request, callback);
}

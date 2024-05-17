bool Browser::CheckMediaAccessPermission(content::WebContents* web_contents,
                                         const GURL& security_origin,
                                         content::MediaStreamType type) {
  return ::CheckMediaAccessPermission(web_contents, security_origin, type);
}

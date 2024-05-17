void RedirectToNtpOrAppsPageIfNecessary(
    content::WebContents* contents,
    signin_metrics::AccessPoint access_point) {
  if (access_point != signin_metrics::AccessPoint::ACCESS_POINT_SETTINGS)
    RedirectToNtpOrAppsPage(contents, access_point);
}

void Browser::PassiveInsecureContentFound(const GURL& resource_url) {
  ReportInsecureContent(SslInsecureContentType::DISPLAY);
  FilteredReportInsecureContentDisplayed(resource_url);
}

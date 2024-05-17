void Browser::EmailPageLocation() {
  UserMetrics::RecordAction(UserMetricsAction("EmailPageLocation"));
  TabContents* tc = GetSelectedTabContents();
  DCHECK(tc);

  std::string title = EscapeQueryParamValue(UTF16ToUTF8(tc->GetTitle()), false);
  std::string page_url = EscapeQueryParamValue(tc->GetURL().spec(), false);
  std::string mailto = std::string("mailto:?subject=Fwd:%20") +
      title + "&body=%0A%0A" + page_url;
  platform_util::OpenExternal(GURL(mailto));
}

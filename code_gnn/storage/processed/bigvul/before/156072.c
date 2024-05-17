const GURL& CommandLineGoogleBaseURL() {
  static base::NoDestructor<std::string> switch_value;
  static base::NoDestructor<GURL> base_url;
  std::string current_switch_value(
      base::CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
          switches::kGoogleBaseURL));
  if (current_switch_value != *switch_value) {
    *switch_value = current_switch_value;
    *base_url = url_formatter::FixupURL(*switch_value, std::string());
    if (!base_url->is_valid() || base_url->has_query() || base_url->has_ref())
      *base_url = GURL();
  }
  return *base_url;
}

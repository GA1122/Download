GURL Extension::GetFullLaunchURL() const {
  return launch_local_path().empty() ? GURL(launch_web_url()) :
                                       url().Resolve(launch_local_path());
}

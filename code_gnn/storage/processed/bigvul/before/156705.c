void SitePerProcessBrowserTest::SetUpCommandLine(
    base::CommandLine* command_line) {
  IsolateAllSitesForTesting(command_line);
#if !defined(OS_ANDROID)
  feature_list_.InitAndDisableFeature(features::kOverlayScrollbar);
#endif
}

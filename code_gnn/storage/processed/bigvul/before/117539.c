void ChromeBrowserMainPartsChromeos::PreBrowserStart() {

  metrics()->StartExternalMetrics();


  g_browser_process->oom_priority_manager()->Start();

  ChromeBrowserMainPartsLinux::PreBrowserStart();
}

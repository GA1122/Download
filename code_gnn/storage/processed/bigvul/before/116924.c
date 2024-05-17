  void RestoreTab(const SessionTab& tab,
                  const int tab_index,
                  Browser* browser,
                  bool schedule_load) {
    DCHECK(!tab.navigations.empty());
    int selected_index = tab.current_navigation_index;
    selected_index = std::max(
        0,
        std::min(selected_index,
                 static_cast<int>(tab.navigations.size() - 1)));

    GURL url = tab.navigations.at(selected_index).virtual_url();
    if (
#if defined(OS_CHROMEOS)
        browser->profile()->GetExtensionService() &&
#endif
        browser->profile()->GetExtensionService()->IsInstalledApp(url)) {
      UMA_HISTOGRAM_ENUMERATION(extension_misc::kAppLaunchHistogram,
                                extension_misc::APP_LAUNCH_SESSION_RESTORE,
                                extension_misc::APP_LAUNCH_BUCKET_BOUNDARY);
    }

    WebContents* web_contents =
        browser->AddRestoredTab(tab.navigations,
                                tab_index,
                                selected_index,
                                tab.extension_app_id,
                                false,
                                tab.pinned,
                                true,
                                NULL);
    if (schedule_load)
      tab_loader_->ScheduleLoad(&web_contents->GetController());
  }

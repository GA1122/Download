  IPC::Message* CreatePpapiClearSiteDataMsg(uint64 max_age) {
    base::FilePath profile_path =
        PepperFlashFileMessageFilter::GetDataDirName(browser_context_path_);
#if defined(OS_WIN)
    base::FilePath plugin_data_path =
        profile_path.Append(base::FilePath(base::UTF8ToUTF16(plugin_name_)));
#else
    base::FilePath plugin_data_path =
        profile_path.Append(base::FilePath(plugin_name_));
#endif   
    return new PpapiMsg_ClearSiteData(0u, plugin_data_path, std::string(),
                                      kClearAllData, max_age);
  }

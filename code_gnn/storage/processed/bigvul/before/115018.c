void TestingAutomationProvider::SendJSONRequest(int handle,
                                                const std::string& json_request,
                                                IPC::Message* reply_message) {
  scoped_ptr<Value> values;
  base::JSONReader reader;
  std::string error;
  values.reset(reader.ReadAndReturnError(json_request, true, NULL, &error));
  if (!error.empty()) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  std::string command;
  DictionaryValue* dict_value = NULL;
  if (values->GetType() != Value::TYPE_DICTIONARY) {
    AutomationJSONReply(this, reply_message).SendError("not a dict");
    return;
  }
  dict_value = static_cast<DictionaryValue*>(values.get());
  if (!dict_value->GetStringASCII(std::string("command"), &command)) {
    AutomationJSONReply(this, reply_message)
        .SendError("no command key in dict or not a string command");
    return;
  }

  std::map<std::string, JsonHandler> handler_map;
  handler_map["WaitForAllTabsToStopLoading"] =
      &TestingAutomationProvider::WaitForAllViewsToStopLoading;
  handler_map["GetIndicesFromTab"] =
      &TestingAutomationProvider::GetIndicesFromTab;
  handler_map["NavigateToURL"] =
      &TestingAutomationProvider::NavigateToURL;
  handler_map["GetLocalStatePrefsInfo"] =
      &TestingAutomationProvider::GetLocalStatePrefsInfo;
  handler_map["SetLocalStatePrefs"] =
      &TestingAutomationProvider::SetLocalStatePrefs;
  handler_map["GetPrefsInfo"] = &TestingAutomationProvider::GetPrefsInfo;
  handler_map["SetPrefs"] = &TestingAutomationProvider::SetPrefs;
  handler_map["ExecuteJavascript"] =
      &TestingAutomationProvider::ExecuteJavascriptJSON;
  handler_map["ExecuteJavascriptInRenderView"] =
      &TestingAutomationProvider::ExecuteJavascriptInRenderView;
  handler_map["GoForward"] =
      &TestingAutomationProvider::GoForward;
  handler_map["GoBack"] =
      &TestingAutomationProvider::GoBack;
  handler_map["Reload"] =
      &TestingAutomationProvider::ReloadJSON;
  handler_map["CaptureEntirePage"] =
      &TestingAutomationProvider::CaptureEntirePageJSON;
  handler_map["GetCookies"] =
      &TestingAutomationProvider::GetCookiesJSON;
  handler_map["DeleteCookie"] =
      &TestingAutomationProvider::DeleteCookieJSON;
  handler_map["SetCookie"] =
      &TestingAutomationProvider::SetCookieJSON;
  handler_map["GetTabIds"] =
      &TestingAutomationProvider::GetTabIds;
  handler_map["GetViews"] =
      &TestingAutomationProvider::GetViews;
  handler_map["IsTabIdValid"] =
      &TestingAutomationProvider::IsTabIdValid;
  handler_map["DoesAutomationObjectExist"] =
      &TestingAutomationProvider::DoesAutomationObjectExist;
  handler_map["CloseTab"] =
      &TestingAutomationProvider::CloseTabJSON;
  handler_map["WebkitMouseMove"] =
      &TestingAutomationProvider::WebkitMouseMove;
  handler_map["WebkitMouseClick"] =
      &TestingAutomationProvider::WebkitMouseClick;
  handler_map["WebkitMouseDrag"] =
      &TestingAutomationProvider::WebkitMouseDrag;
  handler_map["WebkitMouseButtonUp"] =
      &TestingAutomationProvider::WebkitMouseButtonUp;
  handler_map["WebkitMouseButtonDown"] =
      &TestingAutomationProvider::WebkitMouseButtonDown;
  handler_map["WebkitMouseDoubleClick"] =
      &TestingAutomationProvider::WebkitMouseDoubleClick;
  handler_map["DragAndDropFilePaths"] =
      &TestingAutomationProvider::DragAndDropFilePaths;
  handler_map["SendWebkitKeyEvent"] =
      &TestingAutomationProvider::SendWebkitKeyEvent;
  handler_map["SendOSLevelKeyEventToTab"] =
      &TestingAutomationProvider::SendOSLevelKeyEventToTab;
  handler_map["ProcessWebMouseEvent"] =
      &TestingAutomationProvider::ProcessWebMouseEvent;
  handler_map["ActivateTab"] =
      &TestingAutomationProvider::ActivateTabJSON;
  handler_map["GetAppModalDialogMessage"] =
      &TestingAutomationProvider::GetAppModalDialogMessage;
  handler_map["AcceptOrDismissAppModalDialog"] =
      &TestingAutomationProvider::AcceptOrDismissAppModalDialog;
  handler_map["GetChromeDriverAutomationVersion"] =
      &TestingAutomationProvider::GetChromeDriverAutomationVersion;
  handler_map["UpdateExtensionsNow"] =
      &TestingAutomationProvider::UpdateExtensionsNow;
  handler_map["IsPageActionVisible"] =
      &TestingAutomationProvider::IsPageActionVisible;
  handler_map["CreateNewAutomationProvider"] =
      &TestingAutomationProvider::CreateNewAutomationProvider;
  handler_map["GetBrowserInfo"] =
      &TestingAutomationProvider::GetBrowserInfo;
  handler_map["OpenNewBrowserWindowWithNewProfile"] =
      &TestingAutomationProvider::OpenNewBrowserWindowWithNewProfile;
  handler_map["GetMultiProfileInfo"] =
      &TestingAutomationProvider::GetMultiProfileInfo;
  handler_map["GetProcessInfo"] =
      &TestingAutomationProvider::GetProcessInfo;
  handler_map["SetPolicies"] =
      &TestingAutomationProvider::SetPolicies;
  handler_map["GetPolicyDefinitionList"] =
      &TestingAutomationProvider::GetPolicyDefinitionList;
  handler_map["InstallExtension"] =
      &TestingAutomationProvider::InstallExtension;
  handler_map["GetExtensionsInfo"] =
      &TestingAutomationProvider::GetExtensionsInfo;
  handler_map["UninstallExtensionById"] =
      &TestingAutomationProvider::UninstallExtensionById;
  handler_map["SetExtensionStateById"] =
      &TestingAutomationProvider::SetExtensionStateById;
  handler_map["RefreshPolicies"] =
      &TestingAutomationProvider::RefreshPolicies;
  handler_map["TriggerPageActionById"] =
      &TestingAutomationProvider::TriggerPageActionById;
  handler_map["TriggerBrowserActionById"] =
      &TestingAutomationProvider::TriggerBrowserActionById;
#if defined(OS_CHROMEOS)
  handler_map["GetLoginInfo"] = &TestingAutomationProvider::GetLoginInfo;
  handler_map["ShowCreateAccountUI"] =
      &TestingAutomationProvider::ShowCreateAccountUI;
  handler_map["LoginAsGuest"] = &TestingAutomationProvider::LoginAsGuest;
  handler_map["Login"] = &TestingAutomationProvider::Login;

  handler_map["LockScreen"] = &TestingAutomationProvider::LockScreen;
  handler_map["UnlockScreen"] = &TestingAutomationProvider::UnlockScreen;
  handler_map["SignoutInScreenLocker"] =
      &TestingAutomationProvider::SignoutInScreenLocker;

  handler_map["GetBatteryInfo"] = &TestingAutomationProvider::GetBatteryInfo;

  handler_map["GetNetworkInfo"] = &TestingAutomationProvider::GetNetworkInfo;
  handler_map["NetworkScan"] = &TestingAutomationProvider::NetworkScan;
  handler_map["ToggleNetworkDevice"] =
      &TestingAutomationProvider::ToggleNetworkDevice;
  handler_map["ConnectToCellularNetwork"] =
      &TestingAutomationProvider::ConnectToCellularNetwork;
  handler_map["DisconnectFromCellularNetwork"] =
      &TestingAutomationProvider::DisconnectFromCellularNetwork;
  handler_map["ConnectToWifiNetwork"] =
      &TestingAutomationProvider::ConnectToWifiNetwork;
  handler_map["ConnectToHiddenWifiNetwork"] =
      &TestingAutomationProvider::ConnectToHiddenWifiNetwork;
  handler_map["DisconnectFromWifiNetwork"] =
      &TestingAutomationProvider::DisconnectFromWifiNetwork;
  handler_map["ForgetWifiNetwork"] =
      &TestingAutomationProvider::ForgetWifiNetwork;

  handler_map["AddPrivateNetwork"] =
      &TestingAutomationProvider::AddPrivateNetwork;
  handler_map["GetPrivateNetworkInfo"] =
      &TestingAutomationProvider::GetPrivateNetworkInfo;
  handler_map["ConnectToPrivateNetwork"] =
      &TestingAutomationProvider::ConnectToPrivateNetwork;
  handler_map["DisconnectFromPrivateNetwork"] =
      &TestingAutomationProvider::DisconnectFromPrivateNetwork;

  handler_map["IsEnterpriseDevice"] =
      &TestingAutomationProvider::IsEnterpriseDevice;
  handler_map["GetEnterprisePolicyInfo"] =
      &TestingAutomationProvider::GetEnterprisePolicyInfo;
  handler_map["EnrollEnterpriseDevice"] =
      &TestingAutomationProvider::EnrollEnterpriseDevice;

  handler_map["GetTimeInfo"] = &TestingAutomationProvider::GetTimeInfo;
  handler_map["SetTimezone"] = &TestingAutomationProvider::SetTimezone;

  handler_map["GetUpdateInfo"] = &TestingAutomationProvider::GetUpdateInfo;
  handler_map["UpdateCheck"] = &TestingAutomationProvider::UpdateCheck;
  handler_map["SetReleaseTrack"] = &TestingAutomationProvider::SetReleaseTrack;

  handler_map["GetVolumeInfo"] = &TestingAutomationProvider::GetVolumeInfo;
  handler_map["SetVolume"] = &TestingAutomationProvider::SetVolume;
  handler_map["SetMute"] = &TestingAutomationProvider::SetMute;

#endif   

  std::map<std::string, BrowserJsonHandler> browser_handler_map;
  browser_handler_map["DisablePlugin"] =
      &TestingAutomationProvider::DisablePlugin;
  browser_handler_map["EnablePlugin"] =
      &TestingAutomationProvider::EnablePlugin;
  browser_handler_map["GetPluginsInfo"] =
      &TestingAutomationProvider::GetPluginsInfo;

  browser_handler_map["GetNavigationInfo"] =
      &TestingAutomationProvider::GetNavigationInfo;

  browser_handler_map["PerformActionOnInfobar"] =
      &TestingAutomationProvider::PerformActionOnInfobar;

  browser_handler_map["GetHistoryInfo"] =
      &TestingAutomationProvider::GetHistoryInfo;
  browser_handler_map["AddHistoryItem"] =
      &TestingAutomationProvider::AddHistoryItem;

  browser_handler_map["GetOmniboxInfo"] =
      &TestingAutomationProvider::GetOmniboxInfo;
  browser_handler_map["SetOmniboxText"] =
      &TestingAutomationProvider::SetOmniboxText;
  browser_handler_map["OmniboxAcceptInput"] =
      &TestingAutomationProvider::OmniboxAcceptInput;
  browser_handler_map["OmniboxMovePopupSelection"] =
      &TestingAutomationProvider::OmniboxMovePopupSelection;

  browser_handler_map["GetInstantInfo"] =
      &TestingAutomationProvider::GetInstantInfo;

  browser_handler_map["LoadSearchEngineInfo"] =
      &TestingAutomationProvider::LoadSearchEngineInfo;
  browser_handler_map["GetSearchEngineInfo"] =
      &TestingAutomationProvider::GetSearchEngineInfo;
  browser_handler_map["AddOrEditSearchEngine"] =
      &TestingAutomationProvider::AddOrEditSearchEngine;
  browser_handler_map["PerformActionOnSearchEngine"] =
      &TestingAutomationProvider::PerformActionOnSearchEngine;

  browser_handler_map["SetWindowDimensions"] =
      &TestingAutomationProvider::SetWindowDimensions;

  browser_handler_map["GetDownloadsInfo"] =
      &TestingAutomationProvider::GetDownloadsInfo;
  browser_handler_map["WaitForAllDownloadsToComplete"] =
      &TestingAutomationProvider::WaitForAllDownloadsToComplete;
  browser_handler_map["PerformActionOnDownload"] =
      &TestingAutomationProvider::PerformActionOnDownload;

  browser_handler_map["GetInitialLoadTimes"] =
      &TestingAutomationProvider::GetInitialLoadTimes;

  browser_handler_map["SaveTabContents"] =
      &TestingAutomationProvider::SaveTabContents;

  browser_handler_map["ImportSettings"] =
      &TestingAutomationProvider::ImportSettings;

  browser_handler_map["AddSavedPassword"] =
      &TestingAutomationProvider::AddSavedPassword;
  browser_handler_map["RemoveSavedPassword"] =
      &TestingAutomationProvider::RemoveSavedPassword;
  browser_handler_map["GetSavedPasswords"] =
      &TestingAutomationProvider::GetSavedPasswords;

  browser_handler_map["ClearBrowsingData"] =
      &TestingAutomationProvider::ClearBrowsingData;

  browser_handler_map["GetBlockedPopupsInfo"] =
      &TestingAutomationProvider::GetBlockedPopupsInfo;
  browser_handler_map["UnblockAndLaunchBlockedPopup"] =
      &TestingAutomationProvider::UnblockAndLaunchBlockedPopup;

  browser_handler_map["GetThemeInfo"] =
      &TestingAutomationProvider::GetThemeInfo;

  browser_handler_map["FindInPage"] = &TestingAutomationProvider::FindInPage;

  browser_handler_map["SelectTranslateOption"] =
      &TestingAutomationProvider::SelectTranslateOption;
  browser_handler_map["GetTranslateInfo"] =
      &TestingAutomationProvider::GetTranslateInfo;

  browser_handler_map["GetAutofillProfile"] =
      &TestingAutomationProvider::GetAutofillProfile;
  browser_handler_map["FillAutofillProfile"] =
      &TestingAutomationProvider::FillAutofillProfile;
  browser_handler_map["SubmitAutofillForm"] =
      &TestingAutomationProvider::SubmitAutofillForm;
  browser_handler_map["AutofillTriggerSuggestions"] =
      &TestingAutomationProvider::AutofillTriggerSuggestions;
  browser_handler_map["AutofillHighlightSuggestion"] =
      &TestingAutomationProvider::AutofillHighlightSuggestion;
  browser_handler_map["AutofillAcceptSelection"] =
      &TestingAutomationProvider::AutofillAcceptSelection;

  browser_handler_map["GetAllNotifications"] =
      &TestingAutomationProvider::GetAllNotifications;
  browser_handler_map["CloseNotification"] =
      &TestingAutomationProvider::CloseNotification;
  browser_handler_map["WaitForNotificationCount"] =
      &TestingAutomationProvider::WaitForNotificationCount;

  browser_handler_map["SignInToSync"] =
      &TestingAutomationProvider::SignInToSync;
  browser_handler_map["GetSyncInfo"] = &TestingAutomationProvider::GetSyncInfo;
  browser_handler_map["AwaitFullSyncCompletion"] =
      &TestingAutomationProvider::AwaitFullSyncCompletion;
  browser_handler_map["AwaitSyncRestart"] =
      &TestingAutomationProvider::AwaitSyncRestart;
  browser_handler_map["EnableSyncForDatatypes"] =
      &TestingAutomationProvider::EnableSyncForDatatypes;
  browser_handler_map["DisableSyncForDatatypes"] =
      &TestingAutomationProvider::DisableSyncForDatatypes;

  browser_handler_map["GetNTPInfo"] =
      &TestingAutomationProvider::GetNTPInfo;
  browser_handler_map["MoveNTPMostVisitedThumbnail"] =
      &TestingAutomationProvider::MoveNTPMostVisitedThumbnail;
  browser_handler_map["RemoveNTPMostVisitedThumbnail"] =
      &TestingAutomationProvider::RemoveNTPMostVisitedThumbnail;
  browser_handler_map["UnpinNTPMostVisitedThumbnail"] =
      &TestingAutomationProvider::UnpinNTPMostVisitedThumbnail;
  browser_handler_map["RestoreAllNTPMostVisitedThumbnails"] =
      &TestingAutomationProvider::RestoreAllNTPMostVisitedThumbnails;

  browser_handler_map["KillRendererProcess"] =
      &TestingAutomationProvider::KillRendererProcess;

  browser_handler_map["LaunchApp"] = &TestingAutomationProvider::LaunchApp;
  browser_handler_map["SetAppLaunchType"] =
      &TestingAutomationProvider::SetAppLaunchType;
#if defined(OS_CHROMEOS)
  browser_handler_map["CaptureProfilePhoto"] =
      &TestingAutomationProvider::CaptureProfilePhoto;
  browser_handler_map["GetTimeInfo"] = &TestingAutomationProvider::GetTimeInfo;
  browser_handler_map["GetProxySettings"] =
      &TestingAutomationProvider::GetProxySettings;
  browser_handler_map["SetProxySettings"] =
      &TestingAutomationProvider::SetProxySettings;
#endif   

  if (browser_handler_map.find(std::string(command)) !=
             browser_handler_map.end()) {
    Browser* browser = NULL;
    if (!browser_tracker_->ContainsHandle(handle) ||
        !(browser = browser_tracker_->GetResource(handle))) {
      if (handler_map.find(std::string(command)) != handler_map.end())
        (this->*handler_map[command])(dict_value, reply_message);
      else
        AutomationJSONReply(this, reply_message).SendError(
            "No browser object.");
    } else {
      (this->*browser_handler_map[command])(browser, dict_value, reply_message);
    }
  } else if (handler_map.find(std::string(command)) != handler_map.end()) {
    (this->*handler_map[command])(dict_value, reply_message);
  } else {
    std::string error_string = "Unknown command. Options: ";
    for (std::map<std::string, JsonHandler>::const_iterator it =
         handler_map.begin(); it != handler_map.end(); ++it) {
      error_string += it->first + ", ";
    }
    for (std::map<std::string, BrowserJsonHandler>::const_iterator it =
         browser_handler_map.begin(); it != browser_handler_map.end(); ++it) {
      error_string += it->first + ", ";
    }
    AutomationJSONReply(this, reply_message).SendError(error_string);
  }
}

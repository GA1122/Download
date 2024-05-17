bool ExecuteBrowserCommandObserver::CreateAndRegisterObserver(
    AutomationProvider* automation,
    Browser* browser,
    int command,
    IPC::Message* reply_message,
    bool use_json_interface) {
  bool result = true;
  switch (command) {
    case IDC_NEW_TAB: {
      new NewTabObserver(automation, reply_message, use_json_interface);
      break;
    }
    case IDC_NEW_WINDOW:
    case IDC_NEW_INCOGNITO_WINDOW: {
      BrowserOpenedNotificationObserver* observer =
          new BrowserOpenedNotificationObserver(automation, reply_message,
                                                use_json_interface);
      observer->set_for_browser_command(true);
      break;
    }
    case IDC_CLOSE_WINDOW: {
      BrowserClosedNotificationObserver* observer =
          new BrowserClosedNotificationObserver(browser, automation,
                                                reply_message,
                                                use_json_interface);
      observer->set_for_browser_command(true);
      break;
    }
    case IDC_CLOSE_TAB: {
      TabClosedNotificationObserver* observer =
          new TabClosedNotificationObserver(automation, true, reply_message,
                                            use_json_interface);
      observer->set_for_browser_command(true);
      break;
    }
    case IDC_BACK:
    case IDC_FORWARD:
    case IDC_RELOAD: {
      new NavigationNotificationObserver(
          &chrome::GetActiveWebContents(browser)->GetController(),
          automation, reply_message, 1, false, use_json_interface);
      break;
    }
    default: {
      ExecuteBrowserCommandObserver* observer =
          new ExecuteBrowserCommandObserver(automation, reply_message,
                                            use_json_interface);
      if (!observer->Register(command)) {
        observer->ReleaseReply();
        delete observer;
        result = false;
      }
      break;
    }
  }
  return result;
}

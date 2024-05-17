void SessionChangeProcessor::Observe(int type,
                                     const NotificationSource& source,
                                     const NotificationDetails& details) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(running());
  DCHECK(profile_);

  std::vector<SyncedTabDelegate*> modified_tabs;
  switch (type) {
    case chrome::NOTIFICATION_BROWSER_OPENED: {
      Browser* browser = Source<Browser>(source).ptr();
      if (!browser || browser->profile() != profile_) {
        return;
      }
      VLOG(1) << "Received BROWSER_OPENED for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_TAB_PARENTED: {
      SyncedTabDelegate* tab = Source<SyncedTabDelegate>(source).ptr();
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received TAB_PARENTED for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_LOAD_COMPLETED_MAIN_FRAME: {
      TabContentsWrapper* tab_contents_wrapper =
          TabContentsWrapper::GetCurrentWrapperForContents(
              Source<TabContents>(source).ptr());
      if (!tab_contents_wrapper) {
        return;
      }
      SyncedTabDelegate* tab = tab_contents_wrapper->synced_tab_delegate();
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received LOAD_COMPLETED_MAIN_FRAME for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_TAB_CLOSED: {
      SyncedTabDelegate* tab = ExtractSyncedTabDelegate(source);
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received TAB_CLOSED for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_NAV_LIST_PRUNED: {
      SyncedTabDelegate* tab = ExtractSyncedTabDelegate(source);
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received NAV_LIST_PRUNED for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_NAV_ENTRY_CHANGED: {
      SyncedTabDelegate* tab = ExtractSyncedTabDelegate(source);
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received NAV_ENTRY_CHANGED for profile " << profile_;
      break;
    }

    case content::NOTIFICATION_NAV_ENTRY_COMMITTED: {
      SyncedTabDelegate* tab = ExtractSyncedTabDelegate(source);
      if (!tab || tab->profile() != profile_) {
        return;
      }
      modified_tabs.push_back(tab);
      VLOG(1) << "Received NAV_ENTRY_COMMITTED for profile " << profile_;
      break;
    }

    case chrome::NOTIFICATION_TAB_CONTENTS_APPLICATION_EXTENSION_CHANGED: {
      ExtensionTabHelper* extension_tab_helper =
          Source<ExtensionTabHelper>(source).ptr();
      if (!extension_tab_helper ||
          extension_tab_helper->tab_contents()->browser_context() != profile_) {
        return;
      }
      if (extension_tab_helper->extension_app()) {
        modified_tabs.push_back(extension_tab_helper->tab_contents_wrapper()->
            synced_tab_delegate());
      }
      VLOG(1) << "Received TAB_CONTENTS_APPLICATION_EXTENSION_CHANGED "
              << "for profile " << profile_;
      break;
    }

    default:
      LOG(ERROR) << "Received unexpected notification of type "
                  << type;
      break;
  }

  if (!modified_tabs.empty())
    session_model_associator_->ReassociateTabs(modified_tabs);
  session_model_associator_->ReassociateWindows(false);
}

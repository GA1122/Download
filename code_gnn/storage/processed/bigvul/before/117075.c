void SessionService::Observe(int type,
                             const content::NotificationSource& source,
                             const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_BROWSER_OPENED: {
      Browser* browser = content::Source<Browser>(source).ptr();
      if (browser->profile() != profile() ||
          !should_track_changes_for_browser_type(browser->type())) {
        return;
      }

      RestoreIfNecessary(std::vector<GURL>(), browser);
      SetWindowType(browser->session_id(), browser->type());
      break;
    }

    case content::NOTIFICATION_TAB_PARENTED: {
      TabContentsWrapper* tab =
          content::Source<TabContentsWrapper>(source).ptr();
      if (tab->profile() != profile())
        return;
      SetTabWindow(tab->restore_tab_helper()->window_id(),
                   tab->restore_tab_helper()->session_id());
      if (tab->extension_tab_helper()->extension_app()) {
        SetTabExtensionAppID(
            tab->restore_tab_helper()->window_id(),
            tab->restore_tab_helper()->session_id(),
            tab->extension_tab_helper()->extension_app()->id());
      }
      break;
    }

    case content::NOTIFICATION_TAB_CLOSED: {
      TabContentsWrapper* tab =
          TabContentsWrapper::GetCurrentWrapperForContents(
              content::Source<content::NavigationController>(
                  source).ptr()->GetWebContents());
      if (!tab || tab->profile() != profile())
        return;
      TabClosed(tab->restore_tab_helper()->window_id(),
                tab->restore_tab_helper()->session_id(),
                tab->web_contents()->GetClosedByUserGesture());
      RecordSessionUpdateHistogramData(content::NOTIFICATION_TAB_CLOSED,
          &last_updated_tab_closed_time_);
      break;
    }

    case content::NOTIFICATION_NAV_LIST_PRUNED: {
      TabContentsWrapper* tab =
          TabContentsWrapper::GetCurrentWrapperForContents(
              content::Source<content::NavigationController>(
                  source).ptr()->GetWebContents());
      if (!tab || tab->profile() != profile())
        return;
      content::Details<content::PrunedDetails> pruned_details(details);
      if (pruned_details->from_front) {
        TabNavigationPathPrunedFromFront(
            tab->restore_tab_helper()->window_id(),
            tab->restore_tab_helper()->session_id(),
            pruned_details->count);
      } else {
        TabNavigationPathPrunedFromBack(
            tab->restore_tab_helper()->window_id(),
            tab->restore_tab_helper()->session_id(),
            tab->web_contents()->GetController().GetEntryCount());
      }
      RecordSessionUpdateHistogramData(content::NOTIFICATION_NAV_LIST_PRUNED,
          &last_updated_nav_list_pruned_time_);
      break;
    }

    case content::NOTIFICATION_NAV_ENTRY_CHANGED: {
      TabContentsWrapper* tab =
          TabContentsWrapper::GetCurrentWrapperForContents(
              content::Source<content::NavigationController>(
                  source).ptr()->GetWebContents());
      if (!tab || tab->profile() != profile())
        return;
      content::Details<content::EntryChangedDetails> changed(details);
      UpdateTabNavigation(
          tab->restore_tab_helper()->window_id(),
          tab->restore_tab_helper()->session_id(),
          changed->index, *changed->changed_entry);
      break;
    }

    case content::NOTIFICATION_NAV_ENTRY_COMMITTED: {
      TabContentsWrapper* tab =
          TabContentsWrapper::GetCurrentWrapperForContents(
              content::Source<content::NavigationController>(
                  source).ptr()->GetWebContents());
      if (!tab || tab->profile() != profile())
        return;
      int current_entry_index =
          tab->web_contents()->GetController().GetCurrentEntryIndex();
      SetSelectedNavigationIndex(tab->restore_tab_helper()->window_id(),
                                 tab->restore_tab_helper()->session_id(),
                                 current_entry_index);
      UpdateTabNavigation(
          tab->restore_tab_helper()->window_id(),
          tab->restore_tab_helper()->session_id(),
          current_entry_index,
          *tab->web_contents()->GetController().GetEntryAtIndex(
              current_entry_index));
      content::Details<content::LoadCommittedDetails> changed(details);
      if (changed->type == content::NAVIGATION_TYPE_NEW_PAGE ||
        changed->type == content::NAVIGATION_TYPE_EXISTING_PAGE) {
        RecordSessionUpdateHistogramData(
            content::NOTIFICATION_NAV_ENTRY_COMMITTED,
            &last_updated_nav_entry_commit_time_);
      }
      break;
    }

    case chrome::NOTIFICATION_TAB_CONTENTS_APPLICATION_EXTENSION_CHANGED: {
      ExtensionTabHelper* extension_tab_helper =
          content::Source<ExtensionTabHelper>(source).ptr();
      if (extension_tab_helper->tab_contents_wrapper()->profile() != profile())
        return;
      if (extension_tab_helper->extension_app()) {
        RestoreTabHelper* helper =
            extension_tab_helper->tab_contents_wrapper()->restore_tab_helper();
        SetTabExtensionAppID(helper->window_id(),
                             helper->session_id(),
                             extension_tab_helper->extension_app()->id());
      }
      break;
    }

    default:
      NOTREACHED();
  }
}

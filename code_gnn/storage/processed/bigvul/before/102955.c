void TabStripModel::ExecuteContextMenuCommand(
    int context_index, ContextMenuCommand command_id) {
  DCHECK(command_id > CommandFirst && command_id < CommandLast);
  switch (command_id) {
    case CommandNewTab:
      UserMetrics::RecordAction(UserMetricsAction("TabContextMenu_NewTab"),
                                profile_);
      delegate()->AddBlankTabAt(context_index + 1, true);
      break;

    case CommandReload: {
      UserMetrics::RecordAction(UserMetricsAction("TabContextMenu_Reload"),
                                profile_);
      std::vector<int> indices = GetIndicesForCommand(context_index);
      for (size_t i = 0; i < indices.size(); ++i) {
        TabContentsWrapper* tab = GetTabContentsAt(indices[i]);
        if (tab && tab->tab_contents()->delegate()->CanReloadContents(
                tab->tab_contents())) {
          tab->controller().Reload(true);
        }
      }
      break;
    }

    case CommandDuplicate: {
      UserMetrics::RecordAction(UserMetricsAction("TabContextMenu_Duplicate"),
                                profile_);
      std::vector<int> indices = GetIndicesForCommand(context_index);
      std::vector<TabContentsWrapper*> tabs;
      for (size_t i = 0; i < indices.size(); ++i)
        tabs.push_back(GetTabContentsAt(indices[i]));
      for (size_t i = 0; i < tabs.size(); ++i) {
        int index = GetIndexOfTabContents(tabs[i]);
        if (index != -1 && delegate_->CanDuplicateContentsAt(index))
          delegate_->DuplicateContentsAt(index);
      }
      break;
    }

    case CommandCloseTab: {
      UserMetrics::RecordAction(UserMetricsAction("TabContextMenu_CloseTab"),
                                profile_);
      std::vector<int> indices = GetIndicesForCommand(context_index);
      std::vector<TabContentsWrapper*> tabs;
      for (size_t i = 0; i < indices.size(); ++i)
        tabs.push_back(GetTabContentsAt(indices[i]));
      for (size_t i = 0; i < tabs.size() && delegate_->CanCloseTab(); ++i) {
        int index = GetIndexOfTabContents(tabs[i]);
        if (index != -1) {
          CloseTabContentsAt(index,
                             CLOSE_CREATE_HISTORICAL_TAB | CLOSE_USER_GESTURE);
        }
      }
      break;
    }

    case CommandCloseOtherTabs: {
      UserMetrics::RecordAction(
          UserMetricsAction("TabContextMenu_CloseOtherTabs"),
          profile_);
      InternalCloseTabs(GetIndicesClosedByCommand(context_index, command_id),
                        CLOSE_CREATE_HISTORICAL_TAB);
      break;
    }

    case CommandCloseTabsToRight: {
      UserMetrics::RecordAction(
          UserMetricsAction("TabContextMenu_CloseTabsToRight"),
          profile_);
      InternalCloseTabs(GetIndicesClosedByCommand(context_index, command_id),
                        CLOSE_CREATE_HISTORICAL_TAB);
      break;
    }

    case CommandRestoreTab: {
      UserMetrics::RecordAction(UserMetricsAction("TabContextMenu_RestoreTab"),
                                profile_);
      delegate_->RestoreTab();
      break;
    }

    case CommandTogglePinned: {
      UserMetrics::RecordAction(
          UserMetricsAction("TabContextMenu_TogglePinned"),
          profile_);
      std::vector<int> indices = GetIndicesForCommand(context_index);
      bool pin = WillContextMenuPin(context_index);
      if (pin) {
        for (size_t i = 0; i < indices.size(); ++i) {
          if (!IsAppTab(indices[i]))
            SetTabPinned(indices[i], true);
        }
      } else {
        for (size_t i = indices.size(); i > 0; --i) {
          if (!IsAppTab(indices[i - 1]))
            SetTabPinned(indices[i - 1], false);
        }
      }
      break;
    }

    case CommandBookmarkAllTabs: {
      UserMetrics::RecordAction(
          UserMetricsAction("TabContextMenu_BookmarkAllTabs"),
          profile_);

      delegate_->BookmarkAllTabs();
      break;
    }

    case CommandUseVerticalTabs: {
      UserMetrics::RecordAction(
          UserMetricsAction("TabContextMenu_UseVerticalTabs"),
          profile_);

      delegate()->ToggleUseVerticalTabs();
      break;
    }

    case CommandSelectByDomain:
    case CommandSelectByOpener: {
      std::vector<int> indices;
      if (command_id == CommandSelectByDomain)
        GetIndicesWithSameDomain(context_index, &indices);
      else
        GetIndicesWithSameOpener(context_index, &indices);
      TabStripSelectionModel selection_model;
      selection_model.SetSelectedIndex(context_index);
      for (size_t i = 0; i < indices.size(); ++i)
        selection_model.AddIndexToSelection(indices[i]);
      SetSelectionFromModel(selection_model);
      break;
    }

    default:
      NOTREACHED();
  }
}

bool TabStripModel::IsContextMenuCommandEnabled(
    int context_index, ContextMenuCommand command_id) const {
  DCHECK(command_id > CommandFirst && command_id < CommandLast);
  switch (command_id) {
    case CommandNewTab:
    case CommandCloseTab:
      return true;

    case CommandReload: {
      std::vector<int> indices = GetIndicesForCommand(context_index);
      for (size_t i = 0; i < indices.size(); ++i) {
        WebContents* tab = GetWebContentsAt(indices[i]);
        if (tab) {
          CoreTabHelperDelegate* core_delegate =
              CoreTabHelper::FromWebContents(tab)->delegate();
          if (!core_delegate || core_delegate->CanReloadContents(tab))
            return true;
        }
      }
      return false;
    }

    case CommandCloseOtherTabs:
    case CommandCloseTabsToRight:
      return !GetIndicesClosedByCommand(context_index, command_id).empty();

    case CommandDuplicate: {
      std::vector<int> indices = GetIndicesForCommand(context_index);
      for (size_t i = 0; i < indices.size(); ++i) {
        if (delegate_->CanDuplicateContentsAt(indices[i]))
          return true;
      }
      return false;
    }

    case CommandRestoreTab:
      return delegate_->CanRestoreTab();

    case CommandTogglePinned: {
      std::vector<int> indices = GetIndicesForCommand(context_index);
      for (size_t i = 0; i < indices.size(); ++i) {
        if (!IsAppTab(indices[i]))
          return true;
      }
      return false;
    }

    case CommandBookmarkAllTabs:
      return browser_defaults::bookmarks_enabled &&
          delegate_->CanBookmarkAllTabs();

    case CommandSelectByDomain:
    case CommandSelectByOpener:
      return true;

    default:
      NOTREACHED();
  }
  return false;
}

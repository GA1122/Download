bool TabStripModel::IsContextMenuCommandEnabled(
    int context_index, ContextMenuCommand command_id) const {
  DCHECK(command_id > CommandFirst && command_id < CommandLast);
  switch (command_id) {
    case CommandNewTab:
      return true;

    case CommandCloseTab:
      return delegate_->CanCloseTab();

    case CommandReload: {
      std::vector<int> indices = GetIndicesForCommand(context_index);
      for (size_t i = 0; i < indices.size(); ++i) {
        TabContentsWrapper* tab = GetTabContentsAt(indices[i]);
        if (tab && tab->tab_contents()->delegate()->CanReloadContents(
                tab->tab_contents())) {
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

    case CommandUseVerticalTabs:
      return true;

    case CommandSelectByDomain:
    case CommandSelectByOpener:
      return true;

    default:
      NOTREACHED();
  }
  return false;
}

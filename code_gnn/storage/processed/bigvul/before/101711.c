void Browser::OnDidGetApplicationInfo(TabContentsWrapper* source,
                                      int32 page_id) {
  if (GetSelectedTabContentsWrapper() != source)
    return;

  NavigationEntry* entry = source->controller().GetLastCommittedEntry();
  if (!entry || (entry->page_id() != page_id))
    return;

  switch (pending_web_app_action_) {
    case CREATE_SHORTCUT: {
      window()->ShowCreateWebAppShortcutsDialog(source);
      break;
    }
    case UPDATE_SHORTCUT: {
      web_app::UpdateShortcutForTabContents(source);
      break;
    }
    default:
      NOTREACHED();
      break;
  }

  pending_web_app_action_ = NONE;
}

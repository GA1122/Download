string16 LauncherView::GetAccessibleName(const views::View* view) {
  if (!delegate_)
    return string16();
  int view_index = view_model_->GetIndexOfView(view);
  if (view_index == -1)
    return string16();

  switch (model_->items()[view_index].type) {
    case TYPE_TABBED:
    case TYPE_APP_PANEL:
    case TYPE_APP_SHORTCUT:
    case TYPE_PLATFORM_APP:
      return delegate_->GetTitle(model_->items()[view_index]);

    case TYPE_APP_LIST:
      return l10n_util::GetStringUTF16(IDS_AURA_APP_LIST_TITLE);

    case TYPE_BROWSER_SHORTCUT:
      return l10n_util::GetStringUTF16(IDS_AURA_NEW_TAB);
  }
  return string16();
}

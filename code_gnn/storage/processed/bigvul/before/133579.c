const base::string16& WebContentsImpl::GetTitle() const {
  NavigationEntry* entry = controller_.GetTransientEntry();
  std::string accept_languages =
      GetContentClient()->browser()->GetAcceptLangs(
          GetBrowserContext());
  if (entry) {
    return entry->GetTitleForDisplay(accept_languages);
  }
  WebUI* our_web_ui = GetRenderManager()->pending_web_ui() ?
      GetRenderManager()->pending_web_ui() : GetRenderManager()->web_ui();
  if (our_web_ui) {
    entry = controller_.GetVisibleEntry();
    if (!(entry && entry->IsViewSourceMode())) {
      const base::string16& title = our_web_ui->GetOverriddenTitle();
      if (!title.empty())
        return title;
    }
  }

  entry = controller_.GetLastCommittedEntry();

  if (controller_.IsInitialNavigation()) {
    if (entry ||
        (controller_.GetVisibleEntry() &&
         !controller_.GetVisibleEntry()->GetTitle().empty())) {
      entry = controller_.GetVisibleEntry();
    }
  }

  if (entry) {
    return entry->GetTitleForDisplay(accept_languages);
  }

  return page_title_when_no_navigation_entry_;
}
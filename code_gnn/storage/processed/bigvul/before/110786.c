void AutocompleteEditModel::DoPrerender(const AutocompleteMatch& match) {
  if (match.destination_url == PermanentURL())
    return;
  TabContentsWrapper* tab = controller_->GetTabContentsWrapper();
  if (!tab)
    return;
  prerender::PrerenderManager* prerender_manager =
      prerender::PrerenderManagerFactory::GetForProfile(tab->profile());
  if (prerender_manager) {
    content::RenderViewHost* current_host =
        tab->web_contents()->GetRenderViewHost();
    prerender_manager->AddPrerenderFromOmnibox(
        match.destination_url, current_host->GetSessionStorageNamespace());
  }
}

bool RemoveTabFunction::RunImpl() {
  int tab_id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &tab_id));

  Browser* browser = NULL;
  TabContents* contents = NULL;
  if (!GetTabById(tab_id, profile(), include_incognito(),
                  &browser, NULL, &contents, NULL, &error_))
    return false;

  int tab_index = browser->GetIndexOfController(&contents->controller());
  if (browser->tabstrip_model()->IsPhantomTab(tab_index)) {
    error_ = keys::kCannotRemovePhantomTab;
    return false;
  }

  RenderViewHost* render_view_host = contents->render_view_host();
  render_view_host->delegate()->Close(render_view_host);
  return true;
}

void Browser::ConvertContentsToApplication(TabContents* contents) {
  const GURL& url = contents->controller().GetActiveEntry()->url();
  std::string app_name = web_app::GenerateApplicationNameFromURL(url);
  RegisterAppPrefs(app_name, contents->profile());

  DetachContents(contents);
  Browser* app_browser = Browser::CreateForApp(
      app_name, gfx::Size(), profile_, false);
  TabContentsWrapper* wrapper = new TabContentsWrapper(contents);
  app_browser->tabstrip_model()->AppendTabContents(wrapper, true);

  contents->GetMutableRendererPrefs()->can_accept_load_drops = false;
  contents->render_view_host()->SyncRendererPrefs();
  app_browser->window()->Show();
}

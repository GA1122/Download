ScriptExecutor* ExecuteCodeInTabFunction::GetScriptExecutor() {
  Browser* browser = NULL;
  content::WebContents* contents = NULL;

  bool success =
      GetTabById(execute_tab_id_, browser_context(), include_incognito(),
                 &browser, nullptr, &contents, nullptr, &error_) &&
      contents && browser;

  if (!success)
    return NULL;

  return TabHelper::FromWebContents(contents)->script_executor();
}

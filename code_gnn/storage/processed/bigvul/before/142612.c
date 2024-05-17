void TabHelper::OnContentScriptsExecuting(
    content::RenderFrameHost* host,
    const ScriptExecutionObserver::ExecutingScriptsMap& executing_scripts_map,
    const GURL& on_url) {
  FOR_EACH_OBSERVER(
      ScriptExecutionObserver,
      script_execution_observers_,
      OnScriptsExecuted(web_contents(), executing_scripts_map, on_url));
}

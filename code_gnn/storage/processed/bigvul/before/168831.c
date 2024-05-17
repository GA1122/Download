void TabsExecuteScriptFunction::OnExecuteCodeFinished(
    const std::string& error,
    const GURL& on_url,
    const base::ListValue& result) {
  if (error.empty())
    SetResult(result.CreateDeepCopy());
  ExecuteCodeInTabFunction::OnExecuteCodeFinished(error, on_url, result);
}

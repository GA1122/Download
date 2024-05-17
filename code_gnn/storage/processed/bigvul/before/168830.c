void TabsUpdateFunction::OnExecuteCodeFinished(
    const std::string& error,
    const GURL& url,
    const base::ListValue& script_result) {
  if (error.empty())
    PopulateResult();
  else
    error_ = error;
  SendResponse(error.empty());
}

void UpdateCheckCallback(AutomationJSONReply* reply,
                         UpdateEngineClient::UpdateCheckResult result) {
  if (result == UpdateEngineClient::UPDATE_RESULT_SUCCESS)
    reply->SendSuccess(NULL);
  else
    reply->SendError("update check failed");
  delete reply;
}

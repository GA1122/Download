Response TargetHandler::SendMessageToTarget(const std::string& message,
                                            Maybe<std::string> session_id,
                                            Maybe<std::string> target_id) {
  Session* session = nullptr;
  Response response =
      FindSession(std::move(session_id), std::move(target_id), &session, true);
  if (!response.isSuccess())
    return response;
  session->SendMessageToAgentHost(message);
  return Response::OK();
}

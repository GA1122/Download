Response TargetHandler::DetachFromTarget(Maybe<std::string> session_id,
                                         Maybe<std::string> target_id) {
  Session* session = nullptr;
  Response response =
      FindSession(std::move(session_id), std::move(target_id), &session, false);
  if (!response.isSuccess())
    return response;
  session->Detach(false);
  return Response::OK();
}

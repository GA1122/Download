Session* JingleSessionManager::Connect(
    const std::string& host_jid,
    const std::string& host_public_key,
    const std::string& receiver_token,
    CandidateSessionConfig* candidate_config,
    const Session::StateChangeCallback& state_change_callback) {
  DCHECK(CalledOnValidThread());

  JingleSession* jingle_session =
      JingleSession::CreateClientSession(this, host_public_key);
  jingle_session->set_candidate_config(candidate_config);
  jingle_session->set_receiver_token(receiver_token);

  cricket::Session* cricket_session = cricket_session_manager_->CreateSession(
      local_jid_, kChromotingXmlNamespace);

  jingle_session->SetStateChangeCallback(state_change_callback);
  jingle_session->Init(cricket_session);
  sessions_.push_back(jingle_session);

  cricket_session->Initiate(host_jid, CreateClientSessionDescription(
      jingle_session->candidate_config()->Clone(), receiver_token));

  return jingle_session;
}

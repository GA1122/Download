bool JingleSessionManager::AcceptConnection(
    JingleSession* jingle_session,
    cricket::Session* cricket_session) {
  DCHECK(CalledOnValidThread());

  if (closed_) {
    cricket_session->Reject(cricket::STR_TERMINATE_DECLINE);
    return false;
  }

  const cricket::SessionDescription* session_description =
      cricket_session->remote_description();
  const cricket::ContentInfo* content =
      session_description->FirstContentByType(kChromotingXmlNamespace);

  CHECK(content);

  const ContentDescription* content_description =
      static_cast<const ContentDescription*>(content->description);
  jingle_session->set_candidate_config(content_description->config()->Clone());
  jingle_session->set_initiator_token(content_description->auth_token());

  IncomingSessionResponse response = protocol::SessionManager::DECLINE;

  listener_->OnIncomingSession(jingle_session, &response);

  switch (response) {
    case SessionManager::ACCEPT: {
      CandidateSessionConfig* candidate_config =
          CandidateSessionConfig::CreateFrom(jingle_session->config());
      cricket_session->Accept(
          CreateHostSessionDescription(candidate_config,
                                       jingle_session->local_certificate()));
      break;
    }

    case SessionManager::INCOMPATIBLE: {
      cricket_session->TerminateWithReason(
          cricket::STR_TERMINATE_INCOMPATIBLE_PARAMETERS);
      return false;
    }

    case SessionManager::DECLINE: {
      cricket_session->TerminateWithReason(cricket::STR_TERMINATE_DECLINE);
      return false;
    }

    default: {
      NOTREACHED();
    }
  }

  return true;
}

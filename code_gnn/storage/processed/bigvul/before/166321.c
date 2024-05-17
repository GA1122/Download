void SpeechRecognitionManagerImpl::ExecuteTransitionAndGetNextState(
    Session* session, FSMState session_state, FSMEvent event) {
  switch (session_state) {
    case SESSION_STATE_IDLE:
      switch (event) {
        case EVENT_START:
          return SessionStart(*session);
        case EVENT_ABORT:
          return SessionAbort(*session);
        case EVENT_RECOGNITION_ENDED:
          return SessionDelete(session);
        case EVENT_STOP_CAPTURE:
          return SessionStopAudioCapture(*session);
        case EVENT_AUDIO_ENDED:
          return;
      }
      break;
    case SESSION_STATE_CAPTURING_AUDIO:
      switch (event) {
        case EVENT_STOP_CAPTURE:
          return SessionStopAudioCapture(*session);
        case EVENT_ABORT:
          return SessionAbort(*session);
        case EVENT_START:
          return;
        case EVENT_AUDIO_ENDED:
        case EVENT_RECOGNITION_ENDED:
          return NotFeasible(*session, event);
      }
      break;
    case SESSION_STATE_WAITING_FOR_RESULT:
      switch (event) {
        case EVENT_ABORT:
          return SessionAbort(*session);
        case EVENT_AUDIO_ENDED:
          return ResetCapturingSessionId(*session);
        case EVENT_START:
        case EVENT_STOP_CAPTURE:
          return;
        case EVENT_RECOGNITION_ENDED:
          return NotFeasible(*session, event);
      }
      break;
  }
  return NotFeasible(*session, event);
}

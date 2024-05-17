void RTCPeerConnectionHandler::OnaddICECandidateResult(
    const blink::WebRTCVoidRequest& webkit_request, bool result) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::OnaddICECandidateResult");
  if (!result) {
    return webkit_request.RequestFailed(
        webrtc::RTCError(webrtc::RTCErrorType::UNSUPPORTED_OPERATION,
                         std::move("Error processing ICE candidate")));
  }

  return webkit_request.RequestSucceeded();
}

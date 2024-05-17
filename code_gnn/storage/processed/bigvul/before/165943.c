bool RTCPeerConnectionHandler::AddICECandidate(
    const blink::WebRTCVoidRequest& request,
    scoped_refptr<blink::WebRTCICECandidate> candidate) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  TRACE_EVENT0("webrtc", "RTCPeerConnectionHandler::addICECandidate");

  bool result = AddICECandidate(std::move(candidate));
  task_runner_->PostTask(
      FROM_HERE,
      base::BindOnce(&RTCPeerConnectionHandler::OnaddICECandidateResult,
                     weak_factory_.GetWeakPtr(), request, result));
  return true;
}

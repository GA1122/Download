void RTCPeerConnectionHandler::associateWithFrame(blink::WebLocalFrame* frame) {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  DCHECK(frame);
  frame_ = frame;
}

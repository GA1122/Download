void RTCPeerConnectionHandler::ResetUMAStats() {
  DCHECK(task_runner_->RunsTasksInCurrentSequence());
  num_local_candidates_ipv6_ = 0;
  num_local_candidates_ipv4_ = 0;
  ice_connection_checking_start_ = base::TimeTicks();
  memset(ice_state_seen_, 0, sizeof(ice_state_seen_));
}

void ChromotingHost::OnJidChange(const std::string& full_jid) {
  DCHECK(context_->network_message_loop()->BelongsToCurrentThread());
  local_jid_ = full_jid;
}

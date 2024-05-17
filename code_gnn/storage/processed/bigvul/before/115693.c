void HostNPScriptObject::OnNatPolicyUpdate(bool nat_traversal_enabled) {
  if (MessageLoop::current() != host_context_.main_message_loop()) {
    host_context_.main_message_loop()->PostTask(
        FROM_HERE,
        base::Bind(&HostNPScriptObject::OnNatPolicyUpdate,
                   base::Unretained(this), nat_traversal_enabled));
    return;
  }

  VLOG(2) << "OnNatPolicyUpdate: " << nat_traversal_enabled;

  if (nat_traversal_enabled_ && !nat_traversal_enabled) {
    DisconnectInternal();
  }

  {
    base::AutoLock lock(nat_policy_lock_);
    policy_received_ = true;
    nat_traversal_enabled_ = nat_traversal_enabled;
  }

  UpdateWebappNatPolicy(nat_traversal_enabled_);

  if (!pending_connect_.is_null()) {
    pending_connect_.Run();
    pending_connect_.Reset();
  }
}

bool HostNPScriptObject::Init() {
  VLOG(2) << "Init";
  host_context_.Start();
  nat_policy_.reset(
      policy_hack::NatPolicy::Create(host_context_.network_message_loop()));
  nat_policy_->StartWatching(
      base::Bind(&HostNPScriptObject::OnNatPolicyUpdate,
                 base::Unretained(this)));
  return true;
}

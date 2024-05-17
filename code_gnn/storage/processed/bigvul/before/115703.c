void HostNPScriptObject::UpdateWebappNatPolicy(bool nat_traversal_enabled) {
  if (!plugin_message_loop_proxy_->BelongsToCurrentThread()) {
    plugin_message_loop_proxy_->PostTask(
        FROM_HERE, base::Bind(&HostNPScriptObject::UpdateWebappNatPolicy,
                              base::Unretained(this), nat_traversal_enabled));
    return;
  }
  if (on_nat_traversal_policy_changed_func_.get()) {
    NPVariant policy;
    BOOLEAN_TO_NPVARIANT(nat_traversal_enabled, policy);
    InvokeAndIgnoreResult(on_nat_traversal_policy_changed_func_.get(),
                          &policy, 1);
  }
}

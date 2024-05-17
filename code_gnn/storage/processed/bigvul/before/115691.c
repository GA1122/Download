void HostNPScriptObject::NotifyStateChanged(State state) {
  if (!plugin_message_loop_proxy_->BelongsToCurrentThread()) {
    plugin_message_loop_proxy_->PostTask(
        FROM_HERE, base::Bind(&HostNPScriptObject::NotifyStateChanged,
                              base::Unretained(this), state));
    return;
  }
  if (on_state_changed_func_.get()) {
    VLOG(2) << "Calling state changed " << state;
    NPVariant state_var;
    INT32_TO_NPVARIANT(state, state_var);
    bool is_good = InvokeAndIgnoreResult(on_state_changed_func_.get(),
                                         &state_var, 1);
    LOG_IF(ERROR, !is_good) << "OnStateChanged failed";
  }
}

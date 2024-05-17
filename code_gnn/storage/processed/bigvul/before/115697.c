void HostNPScriptObject::PostLogDebugInfo(const std::string& message) {
  if (plugin_message_loop_proxy_->BelongsToCurrentThread()) {
    if (am_currently_logging_)
      return;
  }

  plugin_message_loop_proxy_->PostTask(
      FROM_HERE, base::Bind(&HostNPScriptObject::LogDebugInfo,
                            base::Unretained(this), message));
}

void RenderProcessHostImpl::OnMojoError(int render_process_id,
                                        const std::string& error) {
  LOG(ERROR) << "Terminating render process for bad Mojo message: " << error;

  base::debug::ScopedCrashKey error_key_value("mojo-message-error", error);
  bad_message::ReceivedBadMessage(render_process_id,
                                  bad_message::RPH_MOJO_PROCESS_ERROR);
}

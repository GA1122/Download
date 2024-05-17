void RenderProcessHostImpl::OnMojoError(int render_process_id,
                                        const std::string& error) {
  LOG(ERROR) << "Terminating render process for bad Mojo message: " << error;

  base::debug::ScopedCrashKeyString error_key_value(
      bad_message::GetMojoErrorCrashKey(), error);
  bad_message::ReceivedBadMessage(render_process_id,
                                  bad_message::RPH_MOJO_PROCESS_ERROR);
}

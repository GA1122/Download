error::Error GLES2DecoderImpl::DoCommand(unsigned int command,
                                         unsigned int arg_count,
                                         const void* cmd_data) {
  return DoCommands(1, cmd_data, arg_count + 1, 0);
}

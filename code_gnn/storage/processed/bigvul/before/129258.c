error::Error GLES2DecoderImpl::DoCommand(
    unsigned int command,
    unsigned int arg_count,
    const void* cmd_data) {
  error::Error result = error::kNoError;
  if (log_commands()) {
    LOG(ERROR) << "[" << logger_.GetLogPrefix() << "]" << "cmd: "
               << GetCommandName(command);
  }
  unsigned int command_index = command - kStartPoint - 1;
  if (command_index < arraysize(g_command_info)) {
    const CommandInfo& info = g_command_info[command_index];
    unsigned int info_arg_count = static_cast<unsigned int>(info.arg_count);
    if ((info.arg_flags == cmd::kFixed && arg_count == info_arg_count) ||
        (info.arg_flags == cmd::kAtLeastN && arg_count >= info_arg_count)) {
      bool doing_gpu_trace = false;
      if (gpu_trace_commands_) {
        if (CMD_FLAG_GET_TRACE_LEVEL(info.cmd_flags) <= gpu_trace_level_) {
          doing_gpu_trace = true;
          gpu_tracer_->Begin(GetCommandName(command), kTraceDecoder);
        }
      }

      uint32 immediate_data_size =
          (arg_count - info_arg_count) * sizeof(CommandBufferEntry);   
      switch (command) {
        #define GLES2_CMD_OP(name)                                 \
          case cmds::name::kCmdId:                                 \
            result = Handle ## name(                               \
                immediate_data_size,                               \
                *static_cast<const gles2::cmds::name*>(cmd_data)); \
            break;                                                 \

        GLES2_COMMAND_LIST(GLES2_CMD_OP)
        #undef GLES2_CMD_OP
      }

      if (doing_gpu_trace)
        gpu_tracer_->End(kTraceDecoder);

      if (debug()) {
        GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
          LOG(ERROR) << "[" << logger_.GetLogPrefix() << "] "
                     << "GL ERROR: " << GLES2Util::GetStringEnum(error) << " : "
                     << GetCommandName(command);
          LOCAL_SET_GL_ERROR(error, "DoCommand", "GL error from driver");
        }
      }
    } else {
      result = error::kInvalidArguments;
    }
  } else {
    result = DoCommonCommand(command, arg_count, cmd_data);
  }
  if (result == error::kNoError && current_decoder_error_ != error::kNoError) {
      result = current_decoder_error_;
      current_decoder_error_ = error::kNoError;
  }
  return result;
}
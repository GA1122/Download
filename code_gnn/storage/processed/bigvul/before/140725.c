error::Error GLES2DecoderImpl::DoCommandsImpl(unsigned int num_commands,
                                              const void* buffer,
                                              int num_entries,
                                              int* entries_processed) {
  commands_to_process_ = num_commands;
  error::Error result = error::kNoError;
  const CommandBufferEntry* cmd_data =
      static_cast<const CommandBufferEntry*>(buffer);
  int process_pos = 0;
  unsigned int command = 0;

  while (process_pos < num_entries && result == error::kNoError &&
         commands_to_process_--) {
    const unsigned int size = cmd_data->value_header.size;
    command = cmd_data->value_header.command;

    if (size == 0) {
      result = error::kInvalidSize;
      break;
    }

    if (static_cast<int>(size) + process_pos > num_entries) {
      result = error::kOutOfBounds;
      break;
    }

    if (DebugImpl) {
      TRACE_EVENT_BEGIN0(TRACE_DISABLED_BY_DEFAULT("cb_command"),
                         GetCommandName(command));

      if (log_commands()) {
        LOG(ERROR) << "[" << logger_.GetLogPrefix() << "]"
                   << "cmd: " << GetCommandName(command);
      }
    }

    const unsigned int arg_count = size - 1;
    unsigned int command_index = command - kStartPoint - 1;
    if (command_index < arraysize(command_info)) {
      const CommandInfo& info = command_info[command_index];
      unsigned int info_arg_count = static_cast<unsigned int>(info.arg_count);
      if ((info.arg_flags == cmd::kFixed && arg_count == info_arg_count) ||
          (info.arg_flags == cmd::kAtLeastN && arg_count >= info_arg_count)) {
        bool doing_gpu_trace = false;
        if (DebugImpl && gpu_trace_commands_) {
          if (CMD_FLAG_GET_TRACE_LEVEL(info.cmd_flags) <= gpu_trace_level_) {
            doing_gpu_trace = true;
            gpu_tracer_->Begin(TRACE_DISABLED_BY_DEFAULT("gpu_decoder"),
                               GetCommandName(command),
                               kTraceDecoder);
          }
        }

        uint32 immediate_data_size = (arg_count - info_arg_count) *
                                     sizeof(CommandBufferEntry);   

        result = (this->*info.cmd_handler)(immediate_data_size, cmd_data);

        if (DebugImpl && doing_gpu_trace)
          gpu_tracer_->End(kTraceDecoder);

        if (DebugImpl && debug()) {
          GLenum error;
          while ((error = glGetError()) != GL_NO_ERROR) {
            LOG(ERROR) << "[" << logger_.GetLogPrefix() << "] "
                       << "GL ERROR: " << GLES2Util::GetStringEnum(error)
                       << " : " << GetCommandName(command);
            LOCAL_SET_GL_ERROR(error, "DoCommand", "GL error from driver");
          }
        }
      } else {
        result = error::kInvalidArguments;
      }
    } else {
      result = DoCommonCommand(command, arg_count, cmd_data);
    }

    if (DebugImpl) {
      TRACE_EVENT_END0(TRACE_DISABLED_BY_DEFAULT("cb_command"),
                       GetCommandName(command));
    }

    if (result == error::kNoError &&
        current_decoder_error_ != error::kNoError) {
      result = current_decoder_error_;
      current_decoder_error_ = error::kNoError;
    }

    if (result != error::kDeferCommandUntilLater) {
      process_pos += size;
      cmd_data += size;
    }
  }

  if (entries_processed)
    *entries_processed = process_pos;

  if (error::IsError(result)) {
    LOG(ERROR) << "Error: " << result << " for Command "
               << GetCommandName(command);
  }

  return result;
}

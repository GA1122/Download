GLES2Decoder* GLES2Decoder::Create(
    DecoderClient* client,
    CommandBufferServiceBase* command_buffer_service,
    Outputter* outputter,
    ContextGroup* group) {
  if (group->use_passthrough_cmd_decoder()) {
    return new GLES2DecoderPassthroughImpl(client, command_buffer_service,
                                           outputter, group);
  }
  return new GLES2DecoderImpl(client, command_buffer_service, outputter, group);
}

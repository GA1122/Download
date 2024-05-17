GLES2Decoder::GLES2Decoder(CommandBufferServiceBase* command_buffer_service,
                           Outputter* outputter)
    : CommonDecoder(command_buffer_service), outputter_(outputter) {
  DCHECK(outputter_);
}

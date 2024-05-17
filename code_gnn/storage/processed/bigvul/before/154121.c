GLES2Decoder::GLES2Decoder(DecoderClient* client,
                           CommandBufferServiceBase* command_buffer_service,
                           Outputter* outputter)
    : CommonDecoder(client, command_buffer_service), outputter_(outputter) {
  DCHECK(outputter_);
}

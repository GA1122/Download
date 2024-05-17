TransportTexture::TransportTexture(GpuChannel* channel,
                                   IPC::Message::Sender* sender,
                                   gpu::gles2::GLES2Decoder* decoder,
                                   int32 host_id,
                                   int32 route_id)
  : channel_(channel),
    sender_(sender),
    decoder_(decoder),
    host_id_(host_id),
    route_id_(route_id),
    output_textures_(NULL) {
}

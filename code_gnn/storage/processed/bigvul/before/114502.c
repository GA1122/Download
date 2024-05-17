EglRenderingVDAClient::~EglRenderingVDAClient() {
  DeleteDecoder();   
  CHECK(decoder_deleted());
  STLDeleteValues(&picture_buffers_by_id_);
  SetState(CS_DESTROYED);
}

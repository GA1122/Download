void EglRenderingVDAClient::DeleteDecoder() {
  if (decoder_deleted())
    return;
  decoder_->Destroy();
  decoder_ = NULL;
  STLClearObject(&encoded_data_);
  for (std::set<int>::iterator it = outstanding_texture_ids_.begin();
       it != outstanding_texture_ids_.end(); ++it) {
    rendering_helper_->DeleteTexture(*it);
  }
  outstanding_texture_ids_.clear();
  for (int i = state_ + 1; i < CS_MAX; ++i)
    SetState(static_cast<ClientState>(i));
}

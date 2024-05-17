void GLES2DecoderImpl::DoMultiDrawBeginCHROMIUM(GLsizei drawcount) {
  if (!multi_draw_manager_->Begin(drawcount)) {
    MarkContextLost(error::kGuilty);
    group_->LoseContexts(error::kInnocent);
  }
}

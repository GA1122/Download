void GLES2Implementation::OrderingBarrierCHROMIUM() {
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glOrderingBarrierCHROMIUM");
  helper_->CommandBufferHelper::OrderingBarrier();
}

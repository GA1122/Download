void ClassicPendingScript::Prefinalize() {
  CancelStreaming();
  prefinalizer_called_ = true;
}

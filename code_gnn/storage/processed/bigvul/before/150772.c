void WebBluetoothServiceImpl::CrashRendererAndClosePipe(
    bad_message::BadMessageReason reason) {
  bad_message::ReceivedBadMessage(GetRenderProcessHost(), reason);
  binding_.Close();
}

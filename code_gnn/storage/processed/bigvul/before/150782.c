url::Origin WebBluetoothServiceImpl::GetOrigin() {
  return render_frame_host_->GetLastCommittedOrigin();
}

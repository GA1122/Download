void RenderProcessHostImpl::SetEchoCanceller3(
    bool enable,
    base::OnceCallback<void(bool, const std::string&)> callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!callback.is_null());

  if (!aec3_set_callback_.is_null()) {
    MediaStreamManager::SendMessageToNativeLog("RPHI: Failed to set AEC3");
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::BindOnce(std::move(callback), false,
                                           "Operation already in progress"));
    return;
  }

  aec3_set_callback_ = std::move(callback);
  Send(new AudioProcessingMsg_EnableAec3(enable));
}

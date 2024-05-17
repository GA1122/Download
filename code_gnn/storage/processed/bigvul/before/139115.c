void ValidateRenderFrameId(int render_process_id,
                           int render_frame_id,
                           const base::Callback<void(bool)>& callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  const bool frame_exists = (render_frame_id == kRenderFrameId);
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::Bind(callback, frame_exists));
}

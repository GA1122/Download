void AudioRendererHost::OnRequestDeviceAuthorization(
    int stream_id,
    int render_frame_id,
    int session_id,
    const std::string& device_id,
    const url::Origin& security_origin) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  const base::TimeTicks auth_start_time = base::TimeTicks::Now();
  DVLOG(1) << "AudioRendererHost@" << this << "::OnRequestDeviceAuthorization"
           << "(stream_id=" << stream_id
           << ", render_frame_id=" << render_frame_id
           << ", session_id=" << session_id << ", device_id=" << device_id
           << ", security_origin=" << security_origin << ")";

  if (LookupById(stream_id) || IsAuthorizationStarted(stream_id))
    return;

  authorizations_.insert(
      std::make_pair(stream_id, std::make_pair(false, std::string())));

  authorization_handler_.RequestDeviceAuthorization(
      render_frame_id, session_id, device_id, security_origin,
      base::Bind(&AudioRendererHost::AuthorizationCompleted,
                 base::Unretained(this), stream_id, security_origin,
                 auth_start_time));
}

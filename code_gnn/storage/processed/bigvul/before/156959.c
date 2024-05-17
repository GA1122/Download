blink::WebMediaPlayer::NetworkState WebMediaPlayerMS::GetNetworkState() const {
  DVLOG(1) << __func__ << ", state:" << network_state_;
  DCHECK(thread_checker_.CalledOnValidThread());
  return network_state_;
}

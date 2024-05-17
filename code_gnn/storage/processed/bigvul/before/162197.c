bool VideoCaptureImpl::RemoveClient(int client_id, ClientInfoMap* clients) {
  DCHECK(io_thread_checker_.CalledOnValidThread());

  const ClientInfoMap::iterator it = clients->find(client_id);
  if (it == clients->end())
    return false;

  it->second.state_update_cb.Run(VIDEO_CAPTURE_STATE_STOPPED);
  clients->erase(it);
  return true;
}

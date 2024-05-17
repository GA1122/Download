bool ResourceDispatcherHostImpl::Read(net::URLRequest* request,
                                      int* bytes_read) {
  ResourceRequestInfoImpl* info = ResourceRequestInfoImpl::ForRequest(request);
  DCHECK(!info->is_paused());

  net::IOBuffer* buf;
  int buf_size;
  if (!info->resource_handler()->OnWillRead(info->GetRequestID(),
                                            &buf, &buf_size, -1)) {
    return false;
  }

  DCHECK(buf);
  DCHECK(buf_size > 0);

  info->set_has_started_reading(true);
  return request->Read(buf, buf_size, bytes_read);
}

void GpuMessageFilter::SurfaceUpdated(int32 surface_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  typedef std::vector<linked_ptr<CreateViewCommandBufferRequest> > RequestList;
  RequestList retry_requests;
  retry_requests.swap(pending_requests_);
  for (RequestList::iterator it = retry_requests.begin();
      it != retry_requests.end(); ++it) {
    if ((*it)->surface_id != surface_id) {
      pending_requests_.push_back(*it);
    } else {
      linked_ptr<CreateViewCommandBufferRequest> request = *it;
      OnCreateViewCommandBuffer(request->surface_id,
                                request->init_params,
                                request->reply);
    }
  }
}

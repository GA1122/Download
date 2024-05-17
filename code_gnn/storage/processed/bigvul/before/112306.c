void AbortRequestBeforeItStarts(ResourceMessageFilter* filter,
                                IPC::Message* sync_result,
                                int route_id,
                                int request_id) {
  net::URLRequestStatus status(net::URLRequestStatus::FAILED,
                               net::ERR_ABORTED);
  if (sync_result) {
    SyncLoadResult result;
    result.status = status;
    ResourceHostMsg_SyncLoad::WriteReplyParams(sync_result, result);
    filter->Send(sync_result);
  } else {
    filter->Send(new ResourceMsg_RequestComplete(
        route_id,
        request_id,
        status,
        std::string(),    
        base::TimeTicks()));
  }
}

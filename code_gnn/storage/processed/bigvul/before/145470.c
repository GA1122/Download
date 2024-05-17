void AbortRequestBeforeItStarts(ResourceMessageFilter* filter,
                                IPC::Message* sync_result,
                                int request_id) {
  if (sync_result) {
    SyncLoadResult result;
    result.error_code = net::ERR_ABORTED;
    ResourceHostMsg_SyncLoad::WriteReplyParams(sync_result, result);
    filter->Send(sync_result);
  } else {
    ResourceMsg_RequestCompleteData request_complete_data;
    request_complete_data.error_code = net::ERR_ABORTED;
    request_complete_data.was_ignored_by_handler = false;
    request_complete_data.exists_in_cache = false;
    request_complete_data.completion_time = base::TimeTicks();
    request_complete_data.encoded_data_length = 0;
    filter->Send(new ResourceMsg_RequestComplete(
        request_id, request_complete_data));
  }
}

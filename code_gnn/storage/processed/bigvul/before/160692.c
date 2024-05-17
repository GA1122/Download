void RenderFrameImpl::OnWriteMHTMLToDiskComplete(
    int job_id,
    std::set<std::string> serialized_resources_uri_digests,
    base::TimeDelta main_thread_use_time,
    MhtmlSaveStatus save_status) {
  TRACE_EVENT1("page-serialization",
               "RenderFrameImpl::OnWriteMHTMLToDiskComplete",
               "frame save status", GetMhtmlSaveStatusLabel(save_status));
  DCHECK(RenderThread::Get()) << "Must run in the main renderer thread";
  Send(new FrameHostMsg_SerializeAsMHTMLResponse(
      routing_id_, job_id, save_status, serialized_resources_uri_digests,
      main_thread_use_time));
}

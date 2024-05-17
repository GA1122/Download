void RenderFrameImpl::OnWriteMHTMLComplete(
    SerializeAsMHTMLCallback callback,
    std::unordered_set<std::string> serialized_resources_uri_digests,
    base::TimeDelta main_thread_use_time,
    mojom::MhtmlSaveStatus save_status) {
  TRACE_EVENT1("page-serialization", "RenderFrameImpl::OnWriteMHTMLComplete",
               "frame save status", save_status);
  DCHECK(RenderThread::IsMainThread())
      << "Must run in the main renderer thread";

  std::vector<std::string> digests_of_new_parts(
      std::make_move_iterator(serialized_resources_uri_digests.begin()),
      std::make_move_iterator(serialized_resources_uri_digests.end()));

  std::move(callback).Run(save_status, std::move(digests_of_new_parts),
                          main_thread_use_time);
}

void RenderFrameHostImpl::OnSerializeAsMHTMLResponse(
    int job_id,
    MhtmlSaveStatus save_status,
    const std::set<std::string>& digests_of_uris_of_serialized_resources,
    base::TimeDelta renderer_main_thread_time) {
  MHTMLGenerationManager::GetInstance()->OnSerializeAsMHTMLResponse(
      this, job_id, save_status, digests_of_uris_of_serialized_resources,
      renderer_main_thread_time);
}

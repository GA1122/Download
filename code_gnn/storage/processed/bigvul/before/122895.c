void RenderProcessHostImpl::OnSavedPageAsMHTML(int job_id, int64 data_size) {
  MHTMLGenerationManager::GetInstance()->MHTMLGenerated(job_id, data_size);
}

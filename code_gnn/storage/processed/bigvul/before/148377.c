void WebContentsImpl::GenerateMHTML(
    const MHTMLGenerationParams& params,
    const base::Callback<void(int64_t)>& callback) {
  MHTMLGenerationManager::GetInstance()->SaveMHTML(this, params, callback);
}

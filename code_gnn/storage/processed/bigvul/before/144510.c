void WebContentsImpl::GenerateMHTML(
    const base::FilePath& file,
    const base::Callback<void(int64_t)>& callback) {
  MHTMLGenerationManager::GetInstance()->SaveMHTML(this, file, callback);
}

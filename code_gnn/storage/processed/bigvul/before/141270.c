ukm::UkmRecorder* Document::UkmRecorder() {
  if (ukm_recorder_)
    return ukm_recorder_.get();

  ukm_recorder_ =
      ukm::MojoUkmRecorder::Create(Platform::Current()->GetConnector());

  if (IsInMainFrame())
    ukm_recorder_->UpdateSourceURL(ukm_source_id_, url_);
  return ukm_recorder_.get();
}

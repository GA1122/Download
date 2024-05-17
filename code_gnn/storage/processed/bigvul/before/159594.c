ukm::UkmRecorder* Document::UkmRecorder() {
  DCHECK(IsInMainFrame());

  if (ukm_recorder_)
    return ukm_recorder_.get();

  ukm_recorder_ =
      ukm::MojoUkmRecorder::Create(Platform::Current()->GetConnector());
  ukm_source_id_ = ukm_recorder_->GetNewSourceID();
  ukm_recorder_->UpdateSourceURL(ukm_source_id_, url_);
  return ukm_recorder_.get();
}

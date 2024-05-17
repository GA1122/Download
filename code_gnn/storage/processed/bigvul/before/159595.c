int64_t Document::UkmSourceID() const {
  DCHECK(ukm_recorder_);
  return ukm_source_id_;
}

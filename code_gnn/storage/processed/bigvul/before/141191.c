void Document::RecordUkmOutliveTimeAfterShutdown(int outlive_time_count) {
  if (!needs_to_record_ukm_outlive_time_)
    return;

  DCHECK(ukm_recorder_);
  DCHECK(ukm_source_id_ != ukm::kInvalidSourceId);

  ukm::builders::Document_OutliveTimeAfterShutdown(ukm_source_id_)
      .SetGCCount(outlive_time_count)
      .Record(ukm_recorder_.get());
}

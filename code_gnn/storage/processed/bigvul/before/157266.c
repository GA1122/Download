void WebMediaPlayerImpl::OnMemoryPressure(
    base::MemoryPressureListener::MemoryPressureLevel memory_pressure_level) {
  DVLOG(2) << __func__ << " memory_pressure_level=" << memory_pressure_level;
  DCHECK(main_task_runner_->BelongsToCurrentThread());
  DCHECK(base::FeatureList::IsEnabled(kMemoryPressureBasedSourceBufferGC));
  DCHECK(chunk_demuxer_);

  bool force_instant_gc =
      (enable_instant_source_buffer_gc_ &&
       memory_pressure_level ==
           base::MemoryPressureListener::MEMORY_PRESSURE_LEVEL_CRITICAL);

  media_task_runner_->PostTask(
      FROM_HERE, base::BindOnce(&ChunkDemuxer::OnMemoryPressure,
                                base::Unretained(chunk_demuxer_),
                                base::TimeDelta::FromSecondsD(CurrentTime()),
                                memory_pressure_level, force_instant_gc));
}

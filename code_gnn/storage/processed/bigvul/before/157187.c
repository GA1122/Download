void WebMediaPlayerImpl::DemuxerDestructionHelper(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    std::unique_ptr<Demuxer> demuxer) {
  DCHECK(task_runner->BelongsToCurrentThread());
  base::PostTaskWithTraits(
      FROM_HERE,
      {base::TaskPriority::BEST_EFFORT,
       base::TaskShutdownBehavior::CONTINUE_ON_SHUTDOWN},
      base::BindOnce(
          [](std::unique_ptr<Demuxer> demuxer_to_destroy) {
            SCOPED_UMA_HISTOGRAM_TIMER("Media.MSE.DemuxerDestructionTime");
            demuxer_to_destroy.reset();
          },
          std::move(demuxer)));
}

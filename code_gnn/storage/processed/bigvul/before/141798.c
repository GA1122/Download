void ChromeMetricsServiceClient::OnMemoryDetailCollectionDone() {
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);

  DCHECK(waiting_for_collect_final_metrics_step_);

  base::Closure callback = base::Bind(
      &ChromeMetricsServiceClient::OnHistogramSynchronizationDone,
      weak_ptr_factory_.GetWeakPtr());

  base::TimeDelta timeout =
      base::TimeDelta::FromMilliseconds(kMaxHistogramGatheringWaitDuration);

  DCHECK_EQ(num_async_histogram_fetches_in_progress_, 0);

#if BUILDFLAG(ENABLE_PRINT_PREVIEW) && !defined(OS_CHROMEOS)
  num_async_histogram_fetches_in_progress_ = 3;
  if (!ServiceProcessControl::GetInstance()->GetHistograms(callback, timeout)) {
    DCHECK_EQ(num_async_histogram_fetches_in_progress_, 3);
    --num_async_histogram_fetches_in_progress_;
  }
#else
  num_async_histogram_fetches_in_progress_ = 2;
#endif

  base::PostTaskWithTraitsAndReply(
      FROM_HERE, {content::BrowserThread::UI},
      base::BindOnce(&base::StatisticsRecorder::ImportProvidedHistograms),
      callback);

  content::FetchHistogramsAsynchronously(base::ThreadTaskRunnerHandle::Get(),
                                         callback, timeout);
}

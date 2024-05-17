void DocumentLoader::FinishedLoading(double finish_time) {
  DCHECK(frame_->Loader().StateMachine()->CreatingInitialEmptyDocument() ||
         !frame_->GetPage()->Paused() ||
         MainThreadDebugger::Instance()->IsPaused());

  double response_end_time = finish_time;
  if (!response_end_time)
    response_end_time = time_of_last_data_received_;
  if (!response_end_time)
    response_end_time = MonotonicallyIncreasingTime();
  GetTiming().SetResponseEnd(response_end_time);
  if (!MaybeCreateArchive()) {
    if (!writer_)
      CommitData(0, 0);
  }

  if (!frame_)
    return;

  application_cache_host_->FinishedLoadingMainResource();
  EndWriting();
  ClearMainResourceHandle();
}

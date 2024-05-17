void DocumentLoader::FinishedLoading(TimeTicks finish_time) {
  DCHECK(frame_->Loader().StateMachine()->CreatingInitialEmptyDocument() ||
         !frame_->GetPage()->Paused() ||
         MainThreadDebugger::Instance()->IsPaused());

  TimeTicks response_end_time = finish_time;
  if (response_end_time.is_null())
    response_end_time = time_of_last_data_received_;
  if (response_end_time.is_null())
    response_end_time = CurrentTimeTicks();
  GetTiming().SetResponseEnd(response_end_time);
  if (!MaybeCreateArchive()) {
    if (state_ == kProvisional)
      CommitData(nullptr, 0);
  }

  if (!frame_)
    return;

  application_cache_host_->FinishedLoadingMainResource();
  if (parser_) {
    parser_->Finish();
    parser_.Clear();
  }
  ClearResource();
}

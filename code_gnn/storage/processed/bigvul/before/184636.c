  void ChromotingHost::StopScreenRecorder() {
  DCHECK(MessageLoop::current() == context_->main_message_loop());
//   DCHECK(context_->network_message_loop()->BelongsToCurrentThread());
    DCHECK(recorder_.get());
  
    ++stopping_recorders_;
  recorder_->Stop(base::Bind(&ChromotingHost::OnScreenRecorderStopped, this));
//   scoped_refptr<ScreenRecorder> recorder = recorder_;
    recorder_ = NULL;
//   recorder->Stop(base::Bind(&ChromotingHost::OnScreenRecorderStopped, this));
  }
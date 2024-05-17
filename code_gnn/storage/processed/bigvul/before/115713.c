 void ScreenRecorder::DoStopOnNetworkThread(const base::Closure& done_task) {
   DCHECK(network_loop_->BelongsToCurrentThread());
 
  network_stopped_ = true;

  encode_loop_->PostTask(
      FROM_HERE, base::Bind(&ScreenRecorder::DoStopOnEncodeThread,
                            this, done_task));
}

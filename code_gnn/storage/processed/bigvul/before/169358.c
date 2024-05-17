void MainThreadFrameObserver::Wait() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  render_widget_host_->Send(new ViewMsg_WaitForNextFrameForTests(
      render_widget_host_->GetRoutingID(), routing_id_));
  run_loop_.reset(new base::RunLoop());
  run_loop_->Run();
  run_loop_.reset(nullptr);
}

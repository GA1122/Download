  void CompleteLoading() {
    std::unique_ptr<BackgroundSnapshotController> snapshot_controller(
        new BackgroundSnapshotController(base::ThreadTaskRunnerHandle::Get(),
                                         offliner_.get(),
                                         false  ));
    offliner_->SetBackgroundSnapshotControllerForTest(
        std::move(snapshot_controller));
    offliner()->DocumentAvailableInMainFrame();
    offliner()->DocumentOnLoadCompletedInMainFrame();
    PumpLoop();
  }

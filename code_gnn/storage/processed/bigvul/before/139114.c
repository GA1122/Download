  void SyncWithAudioThread() {
    base::RunLoop().RunUntilIdle();

    base::RunLoop run_loop;
    audio_manager_->GetTaskRunner()->PostTask(
        FROM_HERE, media::BindToCurrentLoop(run_loop.QuitClosure()));
    run_loop.Run();
  }

  void CallCompleteInitialization(MidiResult result) {
    CompleteInitialization(result);
    base::RunLoop run_loop;
    run_loop.RunUntilIdle();
  }

   void HistogramsCallback() {
//   void HistogramsCallback(base::RepeatingClosure on_done) {
      MockHistogramsCallback();
    QuitMessageLoop();
//     on_done.Run();
    }
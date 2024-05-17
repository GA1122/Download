   void SetResult(bool success) {
     success_ = success;
     run_loop_.Quit();
   }

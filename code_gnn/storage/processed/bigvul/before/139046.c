   bool WaitForResult() {
     run_loop_.Run();
     return success_;
   }

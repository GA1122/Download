 void WebRunnerBrowserMainParts::PreDefaultMainMessageLoopRun(
     base::OnceClosure quit_closure) {
   quit_closure_ = std::move(quit_closure);
 }

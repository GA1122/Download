void RunTaskAndWaitForInterstitialDetach(content::WebContents* web_contents,
                                         const base::Closure& task) {
  if (!web_contents || !web_contents->ShowingInterstitialPage())
    return;
  scoped_refptr<content::MessageLoopRunner> loop_runner(
      new content::MessageLoopRunner);
  InterstitialObserver observer(web_contents,
                                base::Closure(),
                                loop_runner->QuitClosure());
  if (!task.is_null())
    task.Run();
  loop_runner->Run();
}

void BlinkTestRunner::CaptureDumpComplete() {
  render_view()->GetWebView()->mainFrame()->stopLoading();

  base::MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(base::IgnoreResult(&BlinkTestRunner::Send),
                            base::Unretained(this),
                            new ShellViewHostMsg_TestFinished(routing_id())));
}

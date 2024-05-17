  void LoadURL() {
    const GURL data_url(kTouchActionDataURL);
    NavigateToURL(shell(), data_url);

    RenderWidgetHostImpl* host = GetWidgetHost();
    scoped_refptr<FrameWatcher> frame_watcher(new FrameWatcher());
    frame_watcher->AttachTo(shell()->web_contents());
    host->GetView()->SetSize(gfx::Size(400, 400));

    base::string16 ready_title(base::ASCIIToUTF16("ready"));
    TitleWatcher watcher(shell()->web_contents(), ready_title);
    ignore_result(watcher.WaitAndGetTitle());

    frame_watcher->WaitFrames(1);
  }

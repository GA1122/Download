  void InitializeWithHTML(const String& html) {
    WebView().MainFrameWidget()->Resize(WebSize(800, 600));

    SimRequest request("https://example.com/test.html", "text/html");
    LoadURL("https://example.com/test.html");
    request.Complete(html);

    UpdateAllLifecyclePhases();
    DCHECK(paint_artifact_compositor());
    paint_artifact_compositor()->EnableExtraDataForTesting();
    UpdateAllLifecyclePhases();

    DCHECK(paint_artifact_compositor()->GetExtraDataForTesting());
  }

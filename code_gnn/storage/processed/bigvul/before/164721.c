  WebFormControlElement GetFormControlElementById(const WebString& id) {
    return GetMainFrame()
        ->GetDocument()
        .GetElementById(id)
        .To<WebFormControlElement>();
  }

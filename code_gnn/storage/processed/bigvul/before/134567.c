  WebDragSourceAura(aura::Window* window, WebContentsImpl* contents)
      : window_(window),
        contents_(contents) {
    base::MessageLoopForUI::current()->AddObserver(this);
    registrar_.Add(this,
                   NOTIFICATION_WEB_CONTENTS_DISCONNECTED,
                   Source<WebContents>(contents));
  }

  LoadWatcher(content::RenderFrame* frame,
              const base::Callback<void(bool)>& callback)
      : content::RenderFrameObserver(frame), callback_(callback) {}

  virtual ~WebDragSourceAura() {
    base::MessageLoopForUI::current()->RemoveObserver(this);
  }

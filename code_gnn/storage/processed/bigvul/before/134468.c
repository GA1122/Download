TabStrip::~TabStrip() {
  FOR_EACH_OBSERVER(TabStripObserver, observers_,
                    TabStripDeleted(this));

  StopAnimating(false);

  DestroyDragController();

  RemoveMessageLoopObserver();

  RemoveAllChildViews(true);
}

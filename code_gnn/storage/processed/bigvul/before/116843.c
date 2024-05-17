    RVHObserver(RenderViewHostObserverArray* parent, RenderViewHost* rvh)
        : content::RenderViewHostObserver(rvh),
          parent_(parent) {
    }

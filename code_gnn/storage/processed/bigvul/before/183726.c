 PassOwnPtr<WebCore::GraphicsContext> LayerTreeCoordinator::beginContentUpdate(const WebCore::IntSize& size, ShareableBitmap::Flags flags, ShareableSurface::Handle& handle, WebCore::IntPoint& offset)
 {
     OwnPtr<WebCore::GraphicsContext> graphicsContext;
     for (unsigned i = 0; i < m_updateAtlases.size(); ++i) {
         UpdateAtlas* atlas = m_updateAtlases[i].get();
         if (atlas->flags() == flags) {
             graphicsContext = atlas->beginPaintingOnAvailableBuffer(handle, size, offset);
             if (graphicsContext)
                 return graphicsContext.release();
         }
     }
  
      static const int ScratchBufferDimension = 1024;  
      m_updateAtlases.append(adoptPtr(new UpdateAtlas(ScratchBufferDimension, flags)));
//     scheduleReleaseInactiveAtlases();
      return m_updateAtlases.last()->beginPaintingOnAvailableBuffer(handle, size, offset);
  }
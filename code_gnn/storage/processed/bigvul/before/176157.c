 AMediaCodecPersistentSurface(
 const sp<IGraphicBufferProducer>& igbp,
 const sp<PersistentSurface>& ps)
 : Surface(igbp) {
        mPersistentSurface = ps;
 }

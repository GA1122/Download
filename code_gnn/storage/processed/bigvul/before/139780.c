 bool FakeOffscreenCanvasSurfaceImpl::GetSurfaceId(cc::SurfaceId* surfaceId)
 {
     *surfaceId = cc::SurfaceId(10, 15, 0);
     return true;
 }

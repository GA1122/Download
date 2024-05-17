void TestController::didFinishLoadForFrame(WKPageRef page, WKFrameRef frame, WKTypeRef, const void* clientInfo)
{
    static_cast<TestController*>(const_cast<void*>(clientInfo))->didFinishLoadForFrame(page, frame);
}

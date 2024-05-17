void RenderThread::ClearPredictorCache() {
  int rv;
  Send(new ViewHostMsg_ClearPredictorCache(&rv));
}

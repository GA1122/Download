void WebSocketExperimentTask::Finish(int result) {
  url_fetcher_.reset();
  scoped_refptr<net::WebSocket> websocket = websocket_;
  websocket_ = NULL;
  callback_->Run(result);
  if (websocket)
    websocket->DetachDelegate();
}

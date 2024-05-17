void WebGraphicsContext3DCommandBufferImpl::deleteQueryEXT(
    WebGLId query) {
  gl_->DeleteQueriesEXT(1, &query);
}

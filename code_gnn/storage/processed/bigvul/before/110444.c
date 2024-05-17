void GLES2DecoderImpl::DeleteQueriesEXTHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    QueryManager::Query* query = query_manager_->GetQuery(client_ids[ii]);
    if (query && !query->IsDeleted()) {
      if (query == current_query_) {
        current_query_ = NULL;
      }
      query->Destroy(true);
      query_manager_->RemoveQuery(client_ids[ii]);
    }
  }
}

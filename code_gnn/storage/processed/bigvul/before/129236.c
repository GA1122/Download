void GLES2DecoderImpl::DeleteQueriesEXTHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    QueryManager::Query* query = query_manager_->GetQuery(client_ids[ii]);
    if (query && !query->IsDeleted()) {
      ContextState::QueryMap::iterator it =
          state_.current_queries.find(query->target());
      if (it != state_.current_queries.end())
        state_.current_queries.erase(it);

      query->Destroy(true);
    }
    query_manager_->RemoveQuery(client_ids[ii]);
  }
}

error::Error GLES2DecoderPassthroughImpl::DoDeleteQueriesEXT(
    GLsizei n,
    const volatile GLuint* queries) {
  if (n < 0) {
    InsertError(GL_INVALID_VALUE, "count cannot be negative.");
    return error::kNoError;
  }

  std::vector<GLuint> queries_copy(queries, queries + n);
  for (GLuint query_client_id : queries_copy) {
    GLuint query_service_id = 0;
    if (!query_id_map_.GetServiceID(query_client_id, &query_service_id) ||
        query_service_id == 0) {
      continue;
    }

    QueryInfo query_info = query_info_map_[query_service_id];
    query_info_map_.erase(query_service_id);

    if (query_info.type == GL_NONE) {
      continue;
    }

    auto active_queries_iter = active_queries_.find(query_info.type);
    if (active_queries_iter != active_queries_.end()) {
      active_queries_.erase(active_queries_iter);
    }

    RemovePendingQuery(query_service_id);
  }
  return DeleteHelper(queries_copy.size(), queries_copy.data(), &query_id_map_,
                      [this](GLsizei n, GLuint* queries) {
                        api()->glDeleteQueriesFn(n, queries);
                      });
}

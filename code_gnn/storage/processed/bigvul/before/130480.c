QueryManager::Query* QueryManager::CreateQuery(
    GLenum target, GLuint client_id, int32 shm_id, uint32 shm_offset) {
  scoped_refptr<Query> query;
  switch (target) {
    case GL_COMMANDS_ISSUED_CHROMIUM:
      query = new CommandsIssuedQuery(this, target, shm_id, shm_offset);
      break;
    case GL_LATENCY_QUERY_CHROMIUM:
      query = new CommandLatencyQuery(this, target, shm_id, shm_offset);
      break;
    case GL_ASYNC_PIXEL_UNPACK_COMPLETED_CHROMIUM:
      query = new AsyncPixelTransfersCompletedQuery(
          this, target, shm_id, shm_offset);
      break;
    case GL_ASYNC_PIXEL_PACK_COMPLETED_CHROMIUM:
      query = new AsyncReadPixelsCompletedQuery(
          this, target, shm_id, shm_offset);
      break;
    case GL_GET_ERROR_QUERY_CHROMIUM:
      query = new GetErrorQuery(this, target, shm_id, shm_offset);
      break;
    default: {
      GLuint service_id = 0;
      glGenQueriesARB(1, &service_id);
      DCHECK_NE(0u, service_id);
      query = new AllSamplesPassedQuery(
          this, target, shm_id, shm_offset, service_id);
      break;
    }
  }
  std::pair<QueryMap::iterator, bool> result =
      queries_.insert(std::make_pair(client_id, query));
  DCHECK(result.second);
  return query.get();
}

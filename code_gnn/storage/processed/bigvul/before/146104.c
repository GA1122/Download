ScriptValue WebGL2RenderingContextBase::getQueryParameter(
    ScriptState* script_state,
    WebGLQuery* query,
    GLenum pname) {
  DCHECK(query);
  bool deleted;
  if (!CheckObjectToBeBound("getQueryParameter", query, deleted))
    return ScriptValue::CreateNull(script_state);
  if (deleted) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getQueryParameter",
                      "attempted to access to a deleted query object");
    return ScriptValue::CreateNull(script_state);
  }

  if (!query->GetTarget()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getQueryParameter",
                      "'query' is not a query object yet, since it has't been "
                      "used by beginQuery");
    return ScriptValue::CreateNull(script_state);
  }
  if (query == current_boolean_occlusion_query_ ||
      query == current_transform_feedback_primitives_written_query_ ||
      query == current_elapsed_query_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getQueryParameter",
                      "query is currently active");
    return ScriptValue::CreateNull(script_state);
  }

  switch (pname) {
    case GL_QUERY_RESULT: {
      query->UpdateCachedResult(ContextGL());
      return WebGLAny(script_state, query->GetQueryResult());
    }
    case GL_QUERY_RESULT_AVAILABLE: {
      query->UpdateCachedResult(ContextGL());
      return WebGLAny(script_state, query->IsQueryResultAvailable());
    }
    default:
      SynthesizeGLError(GL_INVALID_ENUM, "getQueryParameter",
                        "invalid parameter name");
      return ScriptValue::CreateNull(script_state);
  }
}

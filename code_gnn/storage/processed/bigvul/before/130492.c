bool GetErrorQuery::End(base::subtle::Atomic32 submit_count) {
  MarkAsPending(submit_count);
  return MarkAsCompleted(manager()->decoder()->GetErrorState()->GetGLError());
}

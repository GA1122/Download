void Document::postInspectorTask(const WebTraceLocation& location, PassOwnPtr<ExecutionContextTask> task)
{
    m_taskRunner->postInspectorTask(location, task);
}

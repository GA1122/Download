scoped_refptr<WebTaskRunner> Document::GetTaskRunner(TaskType type) {
  DCHECK(IsMainThread());

  if (ContextDocument() && ContextDocument()->GetFrame())
    return ContextDocument()->GetFrame()->GetTaskRunner(type);
  return Platform::Current()->CurrentThread()->GetWebTaskRunner();
}

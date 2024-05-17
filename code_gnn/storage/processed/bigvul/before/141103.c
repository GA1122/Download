scoped_refptr<base::SingleThreadTaskRunner> Document::GetTaskRunner(
    TaskType type) {
  DCHECK(IsMainThread());

  if (ContextDocument() && ContextDocument()->GetFrame())
    return ContextDocument()->GetFrame()->GetTaskRunner(type);
  return Thread::Current()->GetTaskRunner();
}

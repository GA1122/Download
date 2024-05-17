Policy* Document::policy() {
  if (!policy_)
    policy_ = MakeGarbageCollected<DocumentPolicy>(this);
  return policy_.Get();
}

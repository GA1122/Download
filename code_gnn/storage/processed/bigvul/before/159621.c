Document::~Document() {
  DCHECK(GetLayoutViewItem().IsNull());
  DCHECK(!ParentTreeScope());
  DCHECK(!ax_object_cache_);

  InstanceCounters::DecrementCounter(InstanceCounters::kDocumentCounter);
}

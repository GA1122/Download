void Resource::NotifyFinished() {
  DCHECK(IsLoaded());

  ResourceClientWalker<ResourceClient> w(clients_);
  while (ResourceClient* c = w.Next()) {
    MarkClientFinished(c);
    c->NotifyFinished(this);
  }
}

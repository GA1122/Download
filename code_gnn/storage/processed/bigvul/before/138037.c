AXNodeObject* AXNodeObject::create(Node* node,
                                   AXObjectCacheImpl& axObjectCache) {
  return new AXNodeObject(node, axObjectCache);
}

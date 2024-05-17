NodeIterator* Document::createNodeIterator(Node* root,
                                           unsigned what_to_show,
                                           V8NodeFilterCondition* filter) {
  DCHECK(root);
  return NodeIterator::Create(root, what_to_show, filter);
}
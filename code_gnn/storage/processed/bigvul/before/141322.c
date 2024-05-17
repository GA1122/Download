TreeWalker* Document::createTreeWalker(Node* root,
                                       unsigned what_to_show,
                                       V8NodeFilter* filter) {
  DCHECK(root);
  return TreeWalker::Create(root, what_to_show, filter);
}

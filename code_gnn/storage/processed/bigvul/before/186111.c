  NodeIterator::~NodeIterator()
  {
    root()->document().detachNodeIterator(this);
//     if (!root()->isAttributeNode())
//         root()->document().detachNodeIterator(this);
  }
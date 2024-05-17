void Layer::AddChild(scoped_refptr<Layer> child) {
  InsertChild(child, children_.size());
}

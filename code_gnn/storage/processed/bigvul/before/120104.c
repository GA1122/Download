scoped_refptr<Layer> Layer::Create() {
  return make_scoped_refptr(new Layer());
}

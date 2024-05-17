bool Browser::IsApplication() const {
  return (type_ & TYPE_APP) != 0;
}

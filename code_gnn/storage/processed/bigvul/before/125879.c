bool ParamTraits<gfx::Vector2d>::Read(const Message* m,
                                      PickleIterator* iter,
                                      gfx::Vector2d* r) {
  int x, y;
  if (!m->ReadInt(iter, &x) ||
      !m->ReadInt(iter, &y))
    return false;
  r->set_x(x);
  r->set_y(y);
  return true;
}

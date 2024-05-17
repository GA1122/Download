bool ParamTraits<gfx::Vector2dF>::Read(const Message* m,
                                      PickleIterator* iter,
                                      gfx::Vector2dF* r) {
  float x, y;
  if (!ParamTraits<float>::Read(m, iter, &x) ||
      !ParamTraits<float>::Read(m, iter, &y))
    return false;
  r->set_x(x);
  r->set_y(y);
  return true;
}

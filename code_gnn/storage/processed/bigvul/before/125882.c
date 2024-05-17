bool ParamTraits<gfx::RectF>::Read(const Message* m,
                                   PickleIterator* iter,
                                   gfx::RectF* r) {
  float x, y, w, h;
  if (!ParamTraits<float>::Read(m, iter, &x) ||
      !ParamTraits<float>::Read(m, iter, &y) ||
      !ParamTraits<float>::Read(m, iter, &w) ||
      !ParamTraits<float>::Read(m, iter, &h))
    return false;
  r->set_x(x);
  r->set_y(y);
  r->set_width(w);
  r->set_height(h);
  return true;
}

void ParamTraits<gfx::RectF>::Write(Message* m, const gfx::RectF& p) {
  ParamTraits<float>::Write(m, p.x());
  ParamTraits<float>::Write(m, p.y());
  ParamTraits<float>::Write(m, p.width());
  ParamTraits<float>::Write(m, p.height());
}

void ParamTraits<gfx::PointF>::Write(Message* m, const gfx::PointF& v) {
  ParamTraits<float>::Write(m, v.x());
  ParamTraits<float>::Write(m, v.y());
}

void ParamTraits<gfx::Vector2dF>::Write(Message* m, const gfx::Vector2dF& v) {
  ParamTraits<float>::Write(m, v.x());
  ParamTraits<float>::Write(m, v.y());
}

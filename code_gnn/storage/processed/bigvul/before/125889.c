void ParamTraits<gfx::SizeF>::Write(Message* m, const gfx::SizeF& p) {
  ParamTraits<float>::Write(m, p.width());
  ParamTraits<float>::Write(m, p.height());
}

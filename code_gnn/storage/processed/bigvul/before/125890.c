void ParamTraits<gfx::Vector2d>::Write(Message* m, const gfx::Vector2d& v) {
  m->WriteInt(v.x());
  m->WriteInt(v.y());
}

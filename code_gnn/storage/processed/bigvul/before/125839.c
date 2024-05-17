void ParamTraits<Message>::Write(Message* m, const Message& p) {
#if defined(OS_POSIX)
  DCHECK(!p.HasFileDescriptors());
#endif

  m->WriteUInt32(static_cast<uint32>(p.routing_id()));
  m->WriteUInt32(p.type());
  m->WriteUInt32(p.flags());
  m->WriteData(p.payload(), static_cast<uint32>(p.payload_size()));
}

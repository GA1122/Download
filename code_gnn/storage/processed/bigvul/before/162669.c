bool DoPort(const CHAR* spec,
            const Component& port,
            int default_port_for_scheme,
            CanonOutput* output,
            Component* out_port) {
  int port_num = ParsePort(spec, port);
  if (port_num == PORT_UNSPECIFIED || port_num == default_port_for_scheme) {
    *out_port = Component();
    return true;   
  }

  if (port_num == PORT_INVALID) {
    output->push_back(':');
    out_port->begin = output->length();
    AppendInvalidNarrowString(spec, port.begin, port.end(), output);
    out_port->len = output->length() - out_port->begin;
    return false;
  }

  const int buf_size = 6;
  char buf[buf_size];
  WritePortInt(buf, buf_size, port_num);

  output->push_back(':');
  out_port->begin = output->length();
  for (int i = 0; i < buf_size && buf[i]; i++)
    output->push_back(buf[i]);

  out_port->len = output->length() - out_port->begin;
  return true;
}

bool SetIntArrayProperty(XID window,
                         const std::string& name,
                         const std::string& type,
                         const std::vector<int>& value) {
  DCHECK(!value.empty());
  Atom name_atom = GetAtom(name.c_str());
  Atom type_atom = GetAtom(type.c_str());

  scoped_array<long> data(new long[value.size()]);
  for (size_t i = 0; i < value.size(); ++i)
    data[i] = value[i];

  gdk_error_trap_push();
  XChangeProperty(ui::GetXDisplay(),
                  window,
                  name_atom,
                  type_atom,
                  32,   
                  PropModeReplace,
                  reinterpret_cast<const unsigned char*>(data.get()),
                  value.size());   
  XSync(ui::GetXDisplay(), False);
  return gdk_error_trap_pop() == 0;
}

void DataObjectImpl::RemoveData(const FORMATETC& format) {
  if (format.ptd)
    return;   

  for (StoredData::iterator i = contents_.begin(); i != contents_.end(); ++i) {
    if (!(*i)->format_etc.ptd &&
        format.cfFormat == (*i)->format_etc.cfFormat &&
        format.dwAspect == (*i)->format_etc.dwAspect &&
        format.lindex == (*i)->format_etc.lindex &&
        format.tymed == (*i)->format_etc.tymed) {
      contents_.erase(i);
      return;
    }
  }
}

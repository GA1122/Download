bool WebPluginDelegateProxy::UseSynchronousGeometryUpdates() {
  if (info_.name.find(ASCIIToUTF16("Windows Media Player")) != string16::npos)
    return true;

  std::vector<webkit::WebPluginMimeType>::iterator index;
  for (index = info_.mime_types.begin(); index != info_.mime_types.end();
       index++) {
    if (index->mime_type == "application/x-vnd.moveplayer.qm" ||
        index->mime_type == "application/x-vnd.moveplay2.qm" ||
        index->mime_type == "application/x-vnd.movenetworks.qm" ||
        index->mime_type == "application/x-vnd.mnplayer.qm") {
      return true;
    }
  }
  return false;
}
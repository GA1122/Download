NPBool NPN_ConvertPoint(NPP id, double sourceX, double sourceY,
                        NPCoordinateSpace sourceSpace,
                        double *destX, double *destY,
                        NPCoordinateSpace destSpace) {
  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin.get()) {
    return plugin->ConvertPoint(sourceX, sourceY, sourceSpace,
                                destX, destY, destSpace);
  }
  NOTREACHED();
  return FALSE;
}

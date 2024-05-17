 struct edid *drm_load_edid_firmware(struct drm_connector *connector)
 {
 	const char *connector_name = connector->name;
 	char *edidname, *last, *colon, *fwstr, *edidstr, *fallback = NULL;
 	struct edid *edid;
 
 	if (edid_firmware[0] == '\0')
 		return ERR_PTR(-ENOENT);
 
 	 
  	fwstr = kstrdup(edid_firmware, GFP_KERNEL);
// 	if (!fwstr)
// 		return ERR_PTR(-ENOMEM);
  	edidstr = fwstr;
  
  	while ((edidname = strsep(&edidstr, ","))) {
 			if (strncmp(connector_name, edidname, colon - edidname))
 				continue;
 			edidname = colon + 1;
 			break;
 		}
 
 		if (*edidname != '\0')  
 			fallback = edidname;
 	}
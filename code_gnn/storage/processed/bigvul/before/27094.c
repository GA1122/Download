g_NP_Initialize(uint32_t version)
{
  if (g_plugin_NP_Initialize == NULL)
	return NPERR_INVALID_FUNCTABLE_ERROR;

  memset(&plugin_funcs, 0, sizeof(plugin_funcs));
  plugin_funcs.size = sizeof(plugin_funcs);

  memset(&mozilla_funcs, 0, sizeof(mozilla_funcs));
  mozilla_funcs.size = sizeof(mozilla_funcs);
  mozilla_funcs.version = version;
  mozilla_funcs.geturl = g_NPN_GetURL;
  mozilla_funcs.posturl = g_NPN_PostURL;
  mozilla_funcs.requestread = g_NPN_RequestRead;
  mozilla_funcs.newstream = g_NPN_NewStream;
  mozilla_funcs.write = g_NPN_Write;
  mozilla_funcs.destroystream = g_NPN_DestroyStream;
  mozilla_funcs.status = g_NPN_Status;
  mozilla_funcs.uagent = g_NPN_UserAgent;
  mozilla_funcs.memalloc = g_NPN_MemAlloc;
  mozilla_funcs.memfree = g_NPN_MemFree;
  mozilla_funcs.memflush = g_NPN_MemFlush;
  mozilla_funcs.reloadplugins = g_NPN_ReloadPlugins;
  mozilla_funcs.getJavaEnv = g_NPN_GetJavaEnv;
  mozilla_funcs.getJavaPeer = g_NPN_GetJavaPeer;
  mozilla_funcs.geturlnotify = g_NPN_GetURLNotify;
  mozilla_funcs.posturlnotify = g_NPN_PostURLNotify;
  mozilla_funcs.getvalue = g_NPN_GetValue;
  mozilla_funcs.setvalue = g_NPN_SetValue;
  mozilla_funcs.invalidaterect = g_NPN_InvalidateRect;
  mozilla_funcs.invalidateregion = g_NPN_InvalidateRegion;
  mozilla_funcs.forceredraw = g_NPN_ForceRedraw;
  mozilla_funcs.pushpopupsenabledstate = g_NPN_PushPopupsEnabledState;
  mozilla_funcs.poppopupsenabledstate = g_NPN_PopPopupsEnabledState;

  if (NPN_HAS_FEATURE(NPRUNTIME_SCRIPTING)) {
	D(bug(" browser supports scripting through npruntime\n"));
	mozilla_funcs.getstringidentifier = g_NPN_GetStringIdentifier;
	mozilla_funcs.getstringidentifiers = g_NPN_GetStringIdentifiers;
	mozilla_funcs.getintidentifier = g_NPN_GetIntIdentifier;
	mozilla_funcs.identifierisstring = g_NPN_IdentifierIsString;
	mozilla_funcs.utf8fromidentifier = g_NPN_UTF8FromIdentifier;
	mozilla_funcs.intfromidentifier = g_NPN_IntFromIdentifier;
	mozilla_funcs.createobject = g_NPN_CreateObject;
	mozilla_funcs.retainobject = g_NPN_RetainObject;
	mozilla_funcs.releaseobject = g_NPN_ReleaseObject;
	mozilla_funcs.invoke = g_NPN_Invoke;
	mozilla_funcs.invokeDefault = g_NPN_InvokeDefault;
	mozilla_funcs.evaluate = g_NPN_Evaluate;
	mozilla_funcs.getproperty = g_NPN_GetProperty;
	mozilla_funcs.setproperty = g_NPN_SetProperty;
	mozilla_funcs.removeproperty = g_NPN_RemoveProperty;
	mozilla_funcs.hasproperty = g_NPN_HasProperty;
	mozilla_funcs.hasmethod = g_NPN_HasMethod;
	mozilla_funcs.releasevariantvalue = g_NPN_ReleaseVariantValue;
	mozilla_funcs.setexception = g_NPN_SetException;

	if (!npobject_bridge_new())
	  return NPERR_OUT_OF_MEMORY_ERROR;
  }

  NPW_InitializeFuncs(&mozilla_funcs, &plugin_funcs);

  D(bugiI("NP_Initialize\n"));
  NPError ret = g_plugin_NP_Initialize(&mozilla_funcs, &plugin_funcs);
  D(bugiD("NP_Initialize return: %d\n", ret));
  return ret;
}
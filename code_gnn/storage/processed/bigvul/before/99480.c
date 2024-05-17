static bool NPN_Evaluate(NPP npp, NPObject *npObject, NPString *script, NPVariant* result)
{
    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    String scriptString = String::fromUTF8WithLatin1Fallback(script->UTF8Characters, script->UTF8Length);
    
    return plugin->evaluate(npObject, scriptString, result);
}

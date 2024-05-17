static void NPN_Status(NPP npp, const char* message)
{
    String statusbarText;
    if (!message)
        statusbarText = "";
    else
        statusbarText = String::fromUTF8WithLatin1Fallback(message, strlen(message));

    RefPtr<NetscapePlugin> plugin = NetscapePlugin::fromNPP(npp);
    plugin->setStatusbarText(statusbarText);
}

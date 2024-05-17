void WebPageProxy::didReceiveSyncMessage(CoreIPC::Connection* connection, CoreIPC::MessageID messageID, CoreIPC::ArgumentDecoder* arguments, CoreIPC::ArgumentEncoder* reply)
{
#if ENABLE(INSPECTOR)
    if (messageID.is<CoreIPC::MessageClassWebInspectorProxy>()) {
        if (WebInspectorProxy* inspector = this->inspector())
            inspector->didReceiveSyncWebInspectorProxyMessage(connection, messageID, arguments, reply);
        return;
    }
#endif

#if ENABLE(FULLSCREEN_API)
    if (messageID.is<CoreIPC::MessageClassWebFullScreenManagerProxy>()) {
        fullScreenManager()->didReceiveSyncMessage(connection, messageID, arguments, reply);
        return;
    }
#endif

    didReceiveSyncWebPageProxyMessage(connection, messageID, arguments, reply);
}

void FrameLoader::dispatchDocumentElementAvailable()
{
    ScriptForbiddenScope forbidScripts;
    client()->documentElementAvailable();
}

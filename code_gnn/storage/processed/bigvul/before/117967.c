void ScriptController::setIsolatedWorldSecurityOrigin(int worldID, PassRefPtr<SecurityOrigin> securityOrigin)
{
    m_proxy->setIsolatedWorldSecurityOrigin(worldID, securityOrigin);
}

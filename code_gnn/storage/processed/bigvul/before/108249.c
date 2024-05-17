void FrameLoader::dispatchDidClearWindowObjectsInAllWorlds()
{
    if (!m_frame->script()->canExecuteScripts(NotAboutToExecuteScript))
        return;

    Vector<DOMWrapperWorld*> worlds;
    ScriptController::getAllWorlds(worlds);
    for (size_t i = 0; i < worlds.size(); ++i)
        dispatchDidClearWindowObjectInWorld(worlds[i]);
}

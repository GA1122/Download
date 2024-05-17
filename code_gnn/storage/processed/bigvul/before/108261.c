bool FrameLoader::isProcessingUserGesture()
{
    Frame* frame = m_frame->tree()->top();
    if (!frame->script()->canExecuteScripts(NotAboutToExecuteScript))
        return true;  
    return frame->script()->processingUserGesture(mainThreadNormalWorld());  
}

bool FrameLoader::isProcessingUserGesture()
{
    Frame* frame = m_frame->tree()->top();
    if (!frame->script()->isEnabled())
        return true;  
    return frame->script()->processingUserGesture();  
}

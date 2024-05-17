unsigned int webkit_web_frame_number_of_active_animations(WebKitWebFrame* frame)
{
    Frame* coreFrame = core(frame);
    if (!coreFrame)
        return 0;

    AnimationController* controller = coreFrame->animation();
    if (!controller)
        return 0;

    return controller->numberOfActiveAnimations();
}

Frame* Frame::findUnsafeParentScrollPropagationBoundary()
{
    Frame* currentFrame = this;
    Frame* ancestorFrame = tree().parent();

    while (ancestorFrame) {
        if (!ancestorFrame->securityContext()->getSecurityOrigin()->canAccess(securityContext()->getSecurityOrigin()))
            return currentFrame;
        currentFrame = ancestorFrame;
        ancestorFrame = ancestorFrame->tree().parent();
    }
    return nullptr;
}

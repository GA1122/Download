Element* Document::scrollingElement()
{
    if (RuntimeEnabledFeatures::scrollTopLeftInteropEnabled()) {
        if (inQuirksMode()) {
            updateLayoutTreeIfNeeded();
            HTMLBodyElement* body = firstBodyElement();
            if (body && body->layoutObject() && body->layoutObject()->hasOverflowClip())
                return nullptr;

            return body;
        }

        return documentElement();
    }

    return body();
}

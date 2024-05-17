static inline Frame* frameForNode(Node* node)
{
    Node* origNode = node;
    for (; node; node = node->parentNode()) {
        if (RenderObject* renderer = node->renderer()) {
            if (renderer->isRenderView()) {
                if (FrameView* view = toRenderView(renderer)->frameView()) {
                    if (Frame* frame = view->frame())
                        return frame;
                }
            }
            if (renderer->isWidget()) {
                Widget* widget = toRenderWidget(renderer)->widget();
                if (widget && widget->isFrameView()) {
                    if (Frame* frame = static_cast<FrameView*>(widget)->frame())
                        return frame;
                }
            }
        }
    }

    for (node = origNode; node; node = node->parentNode()) {
        if (Document* doc = node->document()) {
            if (Frame* frame = doc->frame())
                return frame;
        }
    }

    return 0;
}

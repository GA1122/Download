static IntRect getNodeWindowRect(Node* node)
{
    if (Frame* frame = frameForNode(node)) {
        if (FrameView* view = frame->view())
            return view->contentsToWindow(node->getRect());
    }
    ASSERT_NOT_REACHED();
    return IntRect();
}

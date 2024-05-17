bool Document::allowExecutingScripts(Node* node)
{
    LocalFrame* frame = executingFrame();
    if (!frame)
        return false;
    if (!node->document().executingFrame())
        return false;
    if (!frame->script().canExecuteScripts(AboutToExecuteScript))
        return false;
    return true;
}

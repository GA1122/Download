int HTMLBodyElement::scrollHeight()
{
    Document& document = this->document();
    document.updateLayoutIgnorePendingStylesheets();
    FrameView* view = document.view();
    return view ? adjustForZoom(view->contentsHeight(), &document) : 0;
}

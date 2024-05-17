int HTMLBodyElement::scrollWidth()
{
    Document& document = this->document();
    document.updateLayoutIgnorePendingStylesheets();
    FrameView* view = document.view();
    return view ? adjustForZoom(view->contentsWidth(), &document) : 0;
}

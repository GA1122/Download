PassRefPtr<FrameView> FrameView::create(LocalFrame* frame)
{
    RefPtr<FrameView> view = adoptRef(new FrameView(frame));
    view->show();
    return view.release();
}

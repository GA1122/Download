void LocalFrame::CreateView(const IntSize& viewport_size,
                            const Color& background_color) {
  DCHECK(this);
  DCHECK(GetPage());

  bool is_local_root = this->IsLocalRoot();

  if (is_local_root && View())
    View()->SetParentVisible(false);

  SetView(nullptr);

  LocalFrameView* frame_view = nullptr;
  if (is_local_root) {
    frame_view = LocalFrameView::Create(*this, viewport_size);

    frame_view->SetLayoutSizeFixedToFrameSize(false);
  } else {
    frame_view = LocalFrameView::Create(*this);
  }

  SetView(frame_view);

  frame_view->UpdateBaseBackgroundColorRecursively(background_color);

  if (is_local_root)
    frame_view->SetParentVisible(true);

  if (OwnerLayoutObject()) {
    HTMLFrameOwnerElement* owner = DeprecatedLocalOwner();
    DCHECK(owner);
    if (owner->ContentFrame() == this)
      owner->SetEmbeddedContentView(frame_view);
  }

  if (Owner())
    View()->SetCanHaveScrollbars(Owner()->ScrollingMode() !=
                                 kScrollbarAlwaysOff);
}

void HTMLFrameOwnerElement::SetEmbeddedContentView(
    EmbeddedContentView* embedded_content_view) {
  if (embedded_content_view == embedded_content_view_)
    return;

  Document* doc = contentDocument();
  if (doc && doc->GetFrame()) {
    bool will_be_display_none = !embedded_content_view;
    if (IsDisplayNone() != will_be_display_none) {
      doc->WillChangeFrameOwnerProperties(
          MarginWidth(), MarginHeight(), ScrollingMode(), will_be_display_none);
    }
  }

  if (embedded_content_view_) {
    if (embedded_content_view_->IsAttached()) {
      embedded_content_view_->DetachFromLayout();
      if (embedded_content_view_->IsPluginView())
        DisposePluginSoon(ToPluginView(embedded_content_view_));
      else
        embedded_content_view_->Dispose();
    }
  }

  embedded_content_view_ = embedded_content_view;
  FrameOwnerPropertiesChanged();

  LayoutEmbeddedContent* layout_embedded_content =
      ToLayoutEmbeddedContent(GetLayoutObject());
  LayoutEmbeddedContentItem layout_embedded_content_item =
      LayoutEmbeddedContentItem(layout_embedded_content);
  if (layout_embedded_content_item.IsNull())
    return;

  if (embedded_content_view_) {
    if (doc) {
      CHECK_NE(doc->Lifecycle().GetState(), DocumentLifecycle::kStopping);
    }
    layout_embedded_content_item.UpdateOnEmbeddedContentViewChange();

    DCHECK_EQ(GetDocument().View(),
              layout_embedded_content_item.GetFrameView());
    DCHECK(layout_embedded_content_item.GetFrameView());
    embedded_content_view_->AttachToLayout();
  }

  if (AXObjectCache* cache = GetDocument().ExistingAXObjectCache())
    cache->ChildrenChanged(layout_embedded_content);
}

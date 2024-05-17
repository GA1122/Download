void FrameView::updateWidgetPositions()
{
    WillBeHeapVector<RefPtrWillBeMember<RenderWidget> > widgets;
    copyToVector(m_widgets, widgets);


    for (size_t i = 0; i < widgets.size() && renderView(); ++i)
        widgets[i]->updateWidgetPosition();

    for (size_t i = 0; i < widgets.size() && renderView(); ++i)
        widgets[i]->widgetPositionsUpdated();
}

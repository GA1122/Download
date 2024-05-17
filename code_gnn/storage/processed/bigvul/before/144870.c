  void PrepareTest(const String& track_style) {
    WebView().Resize(WebSize(200, 200));

    SimRequest request("https://example.com/test.html", "text/html");
    LoadURL("https://example.com/test.html");
    request.Complete(R"HTML(
      <!DOCTYPE html>
        <style>
        ::-webkit-scrollbar {
          width: 10px;
        })HTML" + track_style +
                     R"HTML(
        #d1 {
          position: absolute;
          left: 0;
          right: 0;
          top: 0;
          bottom: 0;
          overflow-x:scroll;
          overflow-y:scroll;
        }
      </style>
      <div id='d1'/>
    )HTML");

    Compositor().BeginFrame();

    Element* div = GetDocument().getElementById("d1");
    ASSERT_TRUE(div);

    ScrollableArea* div_scrollable =
        ToLayoutBox(div->GetLayoutObject())->GetScrollableArea();

    ASSERT_TRUE(div_scrollable->HorizontalScrollbar());
    LayoutScrollbar* horizontal_scrollbar =
        ToLayoutScrollbar(div_scrollable->HorizontalScrollbar());
    horizontal_track_ = horizontal_scrollbar->GetPart(kTrackBGPart);
    ASSERT_TRUE(horizontal_track_);

    ASSERT_TRUE(div_scrollable->VerticalScrollbar());
    LayoutScrollbar* vertical_scrollbar =
        ToLayoutScrollbar(div_scrollable->VerticalScrollbar());
    vertical_track_ = vertical_scrollbar->GetPart(kTrackBGPart);
    ASSERT_TRUE(vertical_track_);
  }

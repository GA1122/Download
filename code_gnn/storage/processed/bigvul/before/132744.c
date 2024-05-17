void ChromotingInstance::OnVideoShape(const webrtc::DesktopRegion& shape) {
  if (desktop_shape_ && shape.Equals(*desktop_shape_))
    return;

  desktop_shape_.reset(new webrtc::DesktopRegion(shape));

  scoped_ptr<base::ListValue> rects_value(new base::ListValue());
  for (webrtc::DesktopRegion::Iterator i(shape); !i.IsAtEnd(); i.Advance()) {
    const webrtc::DesktopRect& rect = i.rect();
    scoped_ptr<base::ListValue> rect_value(new base::ListValue());
    rect_value->AppendInteger(rect.left());
    rect_value->AppendInteger(rect.top());
    rect_value->AppendInteger(rect.width());
    rect_value->AppendInteger(rect.height());
    rects_value->Append(rect_value.release());
  }

  scoped_ptr<base::DictionaryValue> data(new base::DictionaryValue());
  data->Set("rects", rects_value.release());
   PostLegacyJsonMessage("onDesktopShape", data.Pass());
 }

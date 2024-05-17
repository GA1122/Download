void Instance::CreateToolbar(const ToolbarButtonInfo* tb_info, size_t size) {
  toolbar_.reset(new FadingControls());

  DCHECK(tb_info);
  DCHECK(size >= 0);

  current_tb_info_ = tb_info;
  current_tb_info_size_ = size;

  int max_height = 0;
  pp::Point origin(kToolbarFadingOffsetLeft, kToolbarFadingOffsetTop);
  ScalePoint(device_scale_, &origin);

  std::list<Button*> buttons;
  for (size_t i = 0; i < size; i++) {
    Button* btn = new Button;
    pp::ImageData normal_face =
        CreateResourceImage(tb_info[i].normal);
    btn->CreateButton(tb_info[i].id,
                      origin,
                      true,
                      toolbar_.get(),
                      tb_info[i].style,
                      normal_face,
                      CreateResourceImage(tb_info[i].highlighted),
                      CreateResourceImage(tb_info[i].pressed));
    buttons.push_back(btn);

    origin += pp::Point(normal_face.size().width(), 0);
    max_height = std::max(max_height, normal_face.size().height());
  }

  pp::Rect rc_toolbar(0, 0,
      origin.x() + GetToolbarRightOffset(),
      origin.y() + max_height + GetToolbarBottomOffset());
  toolbar_->CreateFadingControls(
      kToolbarId, rc_toolbar, false, this, kTransparentAlpha);

  std::list<Button*>::iterator iter;
  for (iter = buttons.begin(); iter != buttons.end(); ++iter) {
    toolbar_->AddControl(*iter);
  }
}

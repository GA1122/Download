void Shell::PlatformSetContents() {
  CHECK(platform_);
  aura::Window* content = web_contents_->GetView()->GetNativeView();
  aura::Window* parent = platform_->window()->window();
  if (parent->Contains(content))
    return;
  parent->AddChild(content);
  content->Show();
}

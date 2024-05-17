base::WaitableEvent* PluginChannel::GetModalDialogEvent(
    gfx::NativeViewId containing_window) {
   return filter_->GetModalDialogEvent(containing_window);
 }

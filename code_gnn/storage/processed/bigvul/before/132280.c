blink::WebBluetooth* RenderFrameImpl::bluetooth() {
  if (!bluetooth_) {
    bluetooth_.reset(new WebBluetoothImpl(
        ChildThreadImpl::current()->thread_safe_sender(), routing_id_));
  }

   return bluetooth_.get();
 }

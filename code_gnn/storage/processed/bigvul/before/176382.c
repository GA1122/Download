 static bool WasNeutered(JSObject* holder) {
 JSArrayBufferView* view = JSArrayBufferView::cast(holder);
 return view->WasNeutered();
 }

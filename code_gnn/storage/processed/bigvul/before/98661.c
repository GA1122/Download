void RenderWidget::closeWidgetSoon() {

  MessageLoop::current()->PostTask(FROM_HERE, NewRunnableMethod(
      this, &RenderWidget::DoDeferredClose));
}

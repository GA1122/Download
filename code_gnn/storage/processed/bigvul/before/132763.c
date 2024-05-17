bool PepperVideoRenderer2D::Initialize(pp::Instance* instance,
                                       const ClientContext& context,
                                       EventHandler* event_handler) {
  DCHECK(CalledOnValidThread());
  DCHECK(!instance_);
  DCHECK(!event_handler_);
  DCHECK(instance);
  DCHECK(event_handler);

  instance_ = instance;
  event_handler_ = event_handler;
  frame_consumer_proxy_ = new FrameConsumerProxy(
      context.main_task_runner(), weak_factory_.GetWeakPtr());
  software_video_renderer_.reset(new SoftwareVideoRenderer(
      context.main_task_runner(), context.decode_task_runner(),
      frame_consumer_proxy_));

  return true;
}

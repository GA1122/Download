void HTMLCanvasElement::SetPlaceholderFrame(
    scoped_refptr<CanvasResource> image,
    base::WeakPtr<CanvasResourceDispatcher> dispatcher,
    scoped_refptr<base::SingleThreadTaskRunner> task_runner,
    unsigned resource_id) {
  OffscreenCanvasPlaceholder::SetPlaceholderFrame(
      std::move(image), std::move(dispatcher), std::move(task_runner),
      resource_id);
  SetSize(PlaceholderFrame()->Size());
  NotifyListenersCanvasChanged();
}

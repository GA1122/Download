BubbleReference BubbleManager::ShowBubble(
    std::unique_ptr<BubbleDelegate> bubble) {
  DCHECK(thread_checker_.CalledOnValidThread());
  DCHECK_NE(manager_state_, ITERATING_BUBBLES);
  DCHECK(bubble);

  std::unique_ptr<BubbleController> controller(
      new BubbleController(this, std::move(bubble)));

  BubbleReference bubble_ref = controller->AsWeakPtr();

  switch (manager_state_) {
    case SHOW_BUBBLES:
      controller->Show();
      controllers_.push_back(std::move(controller));
      break;
    case NO_MORE_BUBBLES:
      for (auto& observer : observers_)
        observer.OnBubbleNeverShown(controller->AsWeakPtr());
      break;
    default:
      NOTREACHED();
      break;
  }

  return bubble_ref;
}

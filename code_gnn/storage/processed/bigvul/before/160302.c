inline void ImageLoader::EnqueueImageLoadingMicroTask(
    UpdateFromElementBehavior update_behavior,
    ReferrerPolicy referrer_policy) {
  std::unique_ptr<Task> task =
      Task::Create(this, update_behavior, referrer_policy);
  pending_task_ = task->GetWeakPtr();
  Microtask::EnqueueMicrotask(
      WTF::Bind(&Task::Run, WTF::Passed(std::move(task))));
  delay_until_do_update_from_element_ =
      IncrementLoadEventDelayCount::Create(element_->GetDocument());
}

inline void ImageLoader::enqueueImageLoadingMicroTask(
    UpdateFromElementBehavior updateBehavior,
    ReferrerPolicy referrerPolicy) {
  std::unique_ptr<Task> task =
      Task::create(this, updateBehavior, referrerPolicy);
  m_pendingTask = task->createWeakPtr();
  Microtask::enqueueMicrotask(
      WTF::bind(&Task::run, WTF::passed(std::move(task))));
  m_loadDelayCounter =
      IncrementLoadEventDelayCount::create(m_element->document());
}

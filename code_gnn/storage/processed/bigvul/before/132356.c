DeviceManagerImpl::DeviceManagerImpl(
    mojo::InterfaceRequest<DeviceManager> request,
    scoped_ptr<DeviceManagerDelegate> delegate,
    scoped_refptr<base::SequencedTaskRunner> service_task_runner)
    : binding_(this, request.Pass()),
      delegate_(delegate.Pass()),
      service_task_runner_(service_task_runner),
      weak_factory_(this) {
}

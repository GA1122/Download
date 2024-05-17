ResourceDispatcherHostImpl::ResourceDispatcherHostImpl()
    : ResourceDispatcherHostImpl(CreateDownloadHandlerIntercept(),
                                 base::ThreadTaskRunnerHandle::Get(),
                                   true) {}

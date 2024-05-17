 MediaInterfaceProxy::GetMediaInterfaceFactory() {
   DVLOG(1) << __FUNCTION__;
    DCHECK(thread_checker_.CalledOnValidThread());
  
    if (!interface_factory_ptr_)
    ConnectToService();
//     ConnectToMediaService();
  
    DCHECK(interface_factory_ptr_);
  
    return interface_factory_ptr_.get();
  }
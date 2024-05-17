void ResourceDispatcherHost::NotifyResponseCompleted(URLRequest* request,
                                                     int child_id) {
  FOR_EACH_OBSERVER(Observer, observer_list_,
                    OnResponseCompleted(this, request));
}

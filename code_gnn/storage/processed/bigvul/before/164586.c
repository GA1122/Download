MediaStreamDispatcherHost::GetMediaStreamDeviceObserver() {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  if (media_stream_device_observer_)
    return media_stream_device_observer_;

  blink::mojom::MediaStreamDeviceObserverPtr observer;
  auto dispatcher_request = mojo::MakeRequest(&observer);
  observer.set_connection_error_handler(base::BindOnce(
      &MediaStreamDispatcherHost::OnMediaStreamDeviceObserverConnectionError,
      weak_factory_.GetWeakPtr()));
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(&BindMediaStreamDeviceObserverRequest, render_process_id_,
                     render_frame_id_, std::move(dispatcher_request)));
  media_stream_device_observer_ = std::move(observer);
  return media_stream_device_observer_;
}

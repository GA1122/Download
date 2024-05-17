void PushMessagingServiceObserverAndroid::OnMessageHandled() {
  chrome::android::Java_PushMessagingServiceObserver_onMessageHandled(
      base::android::AttachCurrentThread());
}

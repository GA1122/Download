Notification* Notification::create(ExecutionContext* context, const String& title, const NotificationOptions& options, ExceptionState& exceptionState)
{
    if (!RuntimeEnabledFeatures::notificationConstructorEnabled()) {
        exceptionState.throwTypeError("Illegal constructor. Use ServiceWorkerRegistration.showNotification() instead.");
        return nullptr;
    }

    if (context->isServiceWorkerGlobalScope()) {
        exceptionState.throwTypeError("Illegal constructor.");
        return nullptr;
    }

    if (!options.actions().isEmpty()) {
        exceptionState.throwTypeError("Actions are only supported for persistent notifications shown using ServiceWorkerRegistration.showNotification().");
        return nullptr;
    }

    String insecureOriginMessage;
    if (context->isSecureContext(insecureOriginMessage)) {
        UseCounter::count(context, UseCounter::NotificationSecureOrigin);
        if (context->isDocument())
            UseCounter::countCrossOriginIframe(*toDocument(context),  UseCounter::NotificationAPISecureOriginIframe);
    } else {
        UseCounter::count(context, UseCounter::NotificationInsecureOrigin);
        if (context->isDocument())
            UseCounter::countCrossOriginIframe(*toDocument(context),  UseCounter::NotificationAPIInsecureOriginIframe);
    }

    WebNotificationData data = createWebNotificationData(context, title, options, exceptionState);
    if (exceptionState.hadException())
        return nullptr;

    Notification* notification = new Notification(context, data);
    notification->scheduleShow();
    notification->suspendIfNeeded();

    return notification;
}

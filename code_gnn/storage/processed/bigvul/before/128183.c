WebNotificationPermission Notification::checkPermission(ExecutionContext* context)
{
    SecurityOrigin* origin = context->securityOrigin();
    ASSERT(origin);

    return notificationManager()->checkPermission(WebSecurityOrigin(origin));
}

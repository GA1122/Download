String Notification::permission(ExecutionContext* context)
{
    return permissionString(checkPermission(context));
}

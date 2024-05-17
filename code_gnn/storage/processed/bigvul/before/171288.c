static int audit_callback(void *data, __unused security_class_t cls, char *buf, size_t len)
{
 struct audit_data *ad = (struct audit_data *)data;

 if (!ad || !ad->name) {
        ALOGE("No service manager audit data");
 return 0;
 }

    snprintf(buf, len, "service=%s pid=%d uid=%d", ad->name, ad->pid, ad->uid);
 return 0;
}

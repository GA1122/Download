int modbus_set_error_recovery(modbus_t *ctx,
                              modbus_error_recovery_mode error_recovery)
{
    if (ctx == NULL) {
        errno = EINVAL;
        return -1;
    }

     
    ctx->error_recovery = (uint8_t) error_recovery;
    return 0;
}

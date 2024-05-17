static void i6300esb_register_types(void)
{
    watchdog_add_model(&model);
    type_register_static(&i6300esb_info);
}

static int k90_init_macro_functions(struct hid_device *dev)
{
	int ret;
	struct corsair_drvdata *drvdata = hid_get_drvdata(dev);
	struct k90_drvdata *k90;
	size_t name_sz;
	char *name;

	k90 = kzalloc(sizeof(struct k90_drvdata), GFP_KERNEL);
	if (!k90) {
		ret = -ENOMEM;
		goto fail_drvdata;
	}
	drvdata->k90 = k90;

	 
	name_sz = strlen(dev_name(&dev->dev)) + sizeof(K90_RECORD_LED_SUFFIX);
	name = kzalloc(name_sz, GFP_KERNEL);
	if (!name) {
		ret = -ENOMEM;
		goto fail_record_led_alloc;
	}
	snprintf(name, name_sz, "%s" K90_RECORD_LED_SUFFIX,
		 dev_name(&dev->dev));
	k90->record_led.removed = false;
	k90->record_led.cdev.name = name;
	k90->record_led.cdev.max_brightness = 1;
	k90->record_led.cdev.brightness_set = k90_brightness_set;
	k90->record_led.cdev.brightness_get = k90_record_led_get;
	INIT_WORK(&k90->record_led.work, k90_record_led_work);
	k90->record_led.brightness = 0;
	ret = led_classdev_register(&dev->dev, &k90->record_led.cdev);
	if (ret != 0)
		goto fail_record_led;

	 
	ret = sysfs_create_group(&dev->dev.kobj, &k90_attr_group);
	if (ret != 0)
		goto fail_sysfs;

	return 0;

fail_sysfs:
	k90->record_led.removed = true;
	led_classdev_unregister(&k90->record_led.cdev);
	cancel_work_sync(&k90->record_led.work);
fail_record_led:
	kfree(k90->record_led.cdev.name);
fail_record_led_alloc:
	kfree(k90);
fail_drvdata:
	drvdata->k90 = NULL;
	return ret;
}
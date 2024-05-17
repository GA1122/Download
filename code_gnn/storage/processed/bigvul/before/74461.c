static int _regulator_is_enabled(struct regulator_dev *rdev)
{
	 
	if (rdev->ena_pin)
		return rdev->ena_gpio_state;

	 
	if (!rdev->desc->ops->is_enabled)
		return 1;

	return rdev->desc->ops->is_enabled(rdev);
}

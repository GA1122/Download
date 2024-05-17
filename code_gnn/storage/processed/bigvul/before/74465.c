static void drms_uA_update(struct regulator_dev *rdev)
{
	struct regulator *sibling;
	int current_uA = 0, output_uV, input_uV, err;
	unsigned int mode;

	err = regulator_check_drms(rdev);
	if (err < 0 || !rdev->desc->ops->get_optimum_mode ||
	    (!rdev->desc->ops->get_voltage &&
	     !rdev->desc->ops->get_voltage_sel) ||
	    !rdev->desc->ops->set_mode)
		return;

	 
	output_uV = _regulator_get_voltage(rdev);
	if (output_uV <= 0)
		return;

	 
	input_uV = 0;
	if (rdev->supply)
		input_uV = regulator_get_voltage(rdev->supply);
	if (input_uV <= 0)
		input_uV = rdev->constraints->input_uV;
	if (input_uV <= 0)
		return;

	 
	list_for_each_entry(sibling, &rdev->consumer_list, list)
		current_uA += sibling->uA_load;

	 
	mode = rdev->desc->ops->get_optimum_mode(rdev, input_uV,
						  output_uV, current_uA);

	 
	err = regulator_mode_constrain(rdev, &mode);
	if (err == 0)
		rdev->desc->ops->set_mode(rdev, mode);
}

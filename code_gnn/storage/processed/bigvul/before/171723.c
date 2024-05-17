int btif_is_enabled(void)
{
 return ((!btif_is_dut_mode()) && (stack_manager_get_interface()->get_stack_is_running()));
}

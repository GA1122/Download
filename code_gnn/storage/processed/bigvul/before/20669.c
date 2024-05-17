static void emulator_get_fpu(struct x86_emulate_ctxt *ctxt)
{
	preempt_disable();
	kvm_load_guest_fpu(emul_to_vcpu(ctxt));
	 
	clts();
}

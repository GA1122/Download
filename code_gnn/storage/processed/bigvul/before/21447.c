static void __exit exit_elf_binfmt(void)
{
	 
	unregister_binfmt(&elf_format);
}

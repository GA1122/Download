static void print_part_header(const char *type, struct blk_desc *dev_desc)
{
#if CONFIG_IS_ENABLED(MAC_PARTITION) || \
	CONFIG_IS_ENABLED(DOS_PARTITION) || \
	CONFIG_IS_ENABLED(ISO_PARTITION) || \
	CONFIG_IS_ENABLED(AMIGA_PARTITION) || \
	CONFIG_IS_ENABLED(EFI_PARTITION)
	puts ("\nPartition Map for ");
	switch (dev_desc->if_type) {
	case IF_TYPE_IDE:
		puts ("IDE");
		break;
	case IF_TYPE_SATA:
		puts ("SATA");
		break;
	case IF_TYPE_SCSI:
		puts ("SCSI");
		break;
	case IF_TYPE_ATAPI:
		puts ("ATAPI");
		break;
	case IF_TYPE_USB:
		puts ("USB");
		break;
	case IF_TYPE_DOC:
		puts ("DOC");
		break;
	case IF_TYPE_MMC:
		puts ("MMC");
		break;
	case IF_TYPE_HOST:
		puts ("HOST");
		break;
	case IF_TYPE_NVME:
		puts ("NVMe");
		break;
	case IF_TYPE_VIRTIO:
		puts("VirtIO");
		break;
	default:
		puts ("UNKNOWN");
		break;
	}
	printf (" device %d  --   Partition Type: %s\n\n",
			dev_desc->devnum, type);
#endif  
}

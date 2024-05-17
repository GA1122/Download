static int list_algorithms(void)
{
	int i;
	const char *aname = "unknown";

	const id2str_t alg_type_names[] = {
		{ SC_ALGORITHM_RSA,       "rsa"       },
		{ SC_ALGORITHM_DSA,       "dsa"       },
		{ SC_ALGORITHM_EC,        "ec"        },
		{ SC_ALGORITHM_GOSTR3410, "gostr3410" },
		{ SC_ALGORITHM_DES,       "des"       },
		{ SC_ALGORITHM_3DES,      "3des"      },
		{ SC_ALGORITHM_GOST,      "gost"      },
		{ SC_ALGORITHM_MD5,       "md5"       },
		{ SC_ALGORITHM_SHA1,      "sha1"      },
		{ SC_ALGORITHM_GOSTR3411, "gostr3411" },
		{ SC_ALGORITHM_PBKDF2,    "pbkdf2"    },
		{ SC_ALGORITHM_PBES2,     "pbes2"     },
		{ 0, NULL }
	};
	const id2str_t alg_flag_names[] = {
		{ SC_ALGORITHM_ONBOARD_KEY_GEN, "onboard key generation" },
		{ SC_ALGORITHM_NEED_USAGE,      "needs usage"            },
		{ 0, NULL }
	};
	const id2str_t rsa_flag_names[] = {
		{ SC_ALGORITHM_RSA_PAD_PKCS1,      "pkcs1"     },
		{ SC_ALGORITHM_RSA_PAD_ANSI,       "ansi"      },
		{ SC_ALGORITHM_RSA_PAD_PSS,        "pss"       },
		{ SC_ALGORITHM_RSA_PAD_ISO9796,    "iso9796"   },
		{ SC_ALGORITHM_RSA_HASH_SHA1,      "sha1"      },
		{ SC_ALGORITHM_RSA_HASH_MD5,       "MD5"       },
		{ SC_ALGORITHM_RSA_HASH_MD5_SHA1,  "md5-sha1"  },
		{ SC_ALGORITHM_RSA_HASH_RIPEMD160, "ripemd160" },
		{ SC_ALGORITHM_RSA_HASH_SHA256,    "sha256"    },
		{ SC_ALGORITHM_RSA_HASH_SHA384,    "sha384"    },
		{ SC_ALGORITHM_RSA_HASH_SHA512,    "sha512"    },
		{ SC_ALGORITHM_RSA_HASH_SHA224,    "sha224"    },
		{ 0, NULL }
	};

	if (verbose)
		printf("Card supports %d algorithm(s)\n\n",card->algorithm_count);

	for (i=0; i < card->algorithm_count; i++) {
		int j;

		 
		for (j = 0; alg_type_names[j].str != NULL; j++) {
			if (card->algorithms[i].algorithm == alg_type_names[j].id) {
				aname = alg_type_names[j].str;
				break;
			}
		}

		printf("Algorithm: %s\n", aname);
		printf("Key length: %d\n", card->algorithms[i].key_length);
		printf("Flags:");

		 
		for (j = 0; alg_flag_names[j].str != NULL; j++)
			if (card->algorithms[i].flags & alg_flag_names[j].id)
				printf(" %s", alg_flag_names[j].str);

		 
		if ( card->algorithms[i].algorithm == SC_ALGORITHM_RSA) {
			int padding = card->algorithms[i].flags
					& SC_ALGORITHM_RSA_PADS;
			int hashes =  card->algorithms[i].flags
					& SC_ALGORITHM_RSA_HASHES;

			 
			printf(" padding (");
			for (j = 0; rsa_flag_names[j].str != NULL; j++)
				if (padding & rsa_flag_names[j].id)
					printf(" %s", rsa_flag_names[j].str);
			if (padding == SC_ALGORITHM_RSA_PAD_NONE)
				printf(" none");
			printf(" ) ");
			 
			printf("hashes (");
			for (j = 0; rsa_flag_names[j].str != NULL; j++)
				if (hashes & rsa_flag_names[j].id)
					printf(" %s", rsa_flag_names[j].str);
			if (hashes == SC_ALGORITHM_RSA_HASH_NONE)
				printf(" none");
			printf(" )");
		}
		printf("\n");
		if (card->algorithms[i].algorithm == SC_ALGORITHM_RSA
			&& card->algorithms[i].u._rsa.exponent) {
			printf("RSA public exponent: %lu\n", (unsigned long)
				card->algorithms[i].u._rsa.exponent);
		}

		if (i < card->algorithm_count)
			printf("\n");
	}
	return 0;
}
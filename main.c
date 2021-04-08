#include "calc.h"

#include <stdio.h>

#define CAST_DOUBLE 1
#define CAST_FLOAT 2
#define CAST_INTEGER 3
#define CAST_LONG 4

#define OUTPUT_PRINTF 0
#define OUTPUT_BINARY 1
#define OUTPUT_HEX 2

/**
 * Print memory area in binary format
 *
 * @param mem - address of value
 * @param size - size of value
 */
static void print_binary(const unsigned char *mem, int size) {
	char buf[(size + 1) << 3];
	char *p = buf;

	for (; size--; ++mem) {
		int bits = sizeof(char) << 3;

		for (; bits--; ++p) {
			*p = *mem & (1 << bits) ? '1' : '0';
		}

		*(p++) = size ? ' ' : 0;
	}

	printf("%s\n", buf);
}

/**
 * Print memory area in hex values
 *
 * @param mem - address of value
 * @param size - size of value
 */
static void print_hex(const unsigned char *mem, int size) {
	for (; size--; ++mem) {
		printf("%02x ", *mem);
	}
	printf("\n");
}

/**
 * Evaluate given arguments and print the results to stdout
 *
 * @param argc - number of arguments
 * @param argv - arguments
 */
int main(int argc, char **argv) {
	const char *binary = *argv;
	const char *format = "%f\n";
	int cast = CAST_DOUBLE;
	int output = OUTPUT_PRINTF;

	while (--argc) {
		if (**(++argv) == '-') {
			switch (*((*argv) + 1)) {
			default:
				fprintf(stderr, "error: unknown option '%c'\n",
					*((*argv) + 1));
				return -1;
			case 'h':
			case '?':
				fprintf(stdout, "usage: %s [-dflipbx] EXPRESSION...\n"\
					"    -d         result is a double (default)\n"\
					"    -f         cast result to float\n"\
					"    -l         cast result to long\n"\
					"    -i         cast result to int\n"\
					"    -p FORMAT  print result with printf (default)\n"\
					"    -b         print result in binary\n"\
					"    -x         print result as hex dump\n\n",
					binary);
				break;
			case 'i':
				cast = CAST_INTEGER;
				break;
			case 'l':
				cast = CAST_LONG;
				break;
			case 'f':
				cast = CAST_FLOAT;
				break;
			case 'd':
				cast = CAST_DOUBLE;
				break;
			case 'p':
				if (!--argc) {
					fprintf(stderr, "error: missing format argument\n");
					return -1;
				}
				format = *++argv;
				output = OUTPUT_PRINTF;
				break;
			case 'b':
				output = OUTPUT_BINARY;
				break;
			case 'x':
				output = OUTPUT_HEX;
				break;
			}
		} else {
			void *v;
			float f;
			long l;
			int i;
			size_t s;
			double r = 0;

			if (!calc(*argv, &r, 0)) {
				fprintf(stderr, "syntax error\n");
				return -1;
			}

			switch (cast) {
			default:
			case CAST_DOUBLE:
				v = &r;
				s = sizeof(double);
				break;
			case CAST_FLOAT:
				f = (float) r;
				v = &f;
				s = sizeof(float);
				break;
			case CAST_LONG:
				l = (long) r;
				v = &l;
				s = sizeof(long);
				break;
			case CAST_INTEGER:
				i = (int) r;
				v = &i;
				s = sizeof(int);
				break;
			}

			switch (output) {
			default:
			case OUTPUT_PRINTF:
				switch (cast) {
				default:
				case CAST_DOUBLE:
					printf(format, r);
					break;
				case CAST_FLOAT:
					printf(format, f);
					break;
				case CAST_LONG:
					printf(format, l);
					break;
				case CAST_INTEGER:
					printf(format, i);
					break;
				}
				break;
			case OUTPUT_BINARY:
				print_binary((unsigned char *) v, s);
				break;
			case OUTPUT_HEX:
				print_hex((unsigned char *) v, s);
				break;
			}
		}
	}

	return 0;
}

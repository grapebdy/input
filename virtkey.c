#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int key_write(const char *filename, unsigned char buff)
{
        int fd = -1;
        int rd_num;
        fd = open(filename, O_RDWR | O_SYNC);
        if (fd < 0) {
                fprintf(stderr, "cannot open %s\n", filename);
                return fd;
        }

        rd_num = write(fd, &buff, 1);

        close(fd);
        return rd_num;
}

void do_usage(void)
{
        printf("Usage: virtkey cmd [OPERAND]...\n");
        printf("EEPROM test program\n");
        printf("Command:\n");
        printf("\t-f filename\t\t virtkey input file(dmesg)\n");
        printf("\t-k keycode\t\t Write key code\n");

}

int main(int argc, char *argv[])
{
	int opt;
	int keycode =28; // enter
	int fileno = 4;	 // input device 4
	char filename[128];
	if (argc < 3) {
		do_usage();
		return -1;
	}

	while ((opt = getopt(argc,argv,"f:k:")) !=-1) {
		switch (opt) {
		case 'f':
			fileno = atoi(optarg);
			break;
		case 'k':
			keycode = atoi(optarg);
			break;
		default:
			do_usage();
			return -1;
		}
	}

	sprintf(filename, "%s%d%s", "/sys/devices/virtual/input/input", fileno, "/virtkey");
	printf("%s:%d \n", filename, keycode);
	key_write(filename, keycode);

	return 0;
}

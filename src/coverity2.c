#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

void readTest() {
  int fd = open("test.bin", O_RDONLY);
  if (fd == -1) return;
  uint32_t id[2];
  size_t size = read(fd, (void*) id, sizeof(id));
  if (size == sizeof(id))
    printf("%d, %d\n", id[0], id[1]);
  close(fd);
}

int main(void) {
  readTest();
	FILE *fd = fopen("test.bin", "rb");
	int id[2];
	size_t size = fread((void*) id, sizeof(int), 2, fd);
	if (size == 2)
		printf("%d, %d\n", id[0], id[1]);
	fclose(fd);
	return EXIT_SUCCESS;
}

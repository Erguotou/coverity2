#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <curl/curl.h>

typedef unsigned char uuid_t[16];
struct uuid {
  uint32_t  time_low;
  uint16_t  time_mid;
  uint16_t  time_hi_and_version;
  uint16_t  clock_seq;
  uint8_t node[6];
};

void uuid_unpack(const uuid_t in, struct uuid *uu)
{
  const uint8_t *ptr = in;
  uint32_t    tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  tmp = (tmp << 8) | *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_low = tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_mid = tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->time_hi_and_version = tmp;

  tmp = *ptr++;
  tmp = (tmp << 8) | *ptr++;
  uu->clock_seq = tmp;

  memcpy(uu->node, ptr, 6);
}
static void uuid_unparse_x(const uuid_t uu, char *out, const char *fmt)
{
  struct uuid uuid;

  uuid_unpack(uu, &uuid);
  sprintf(out, fmt,
    uuid.time_low, uuid.time_mid, uuid.time_hi_and_version,
    uuid.clock_seq >> 8, uuid.clock_seq & 0xFF,
    uuid.node[0], uuid.node[1], uuid.node[2],
    uuid.node[3], uuid.node[4], uuid.node[5]);
}
static const char *fmt_lower =
    "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";
void uuid_unparse(const uuid_t uu, char *out)
{
  uuid_unparse_x(uu, out, fmt_lower);
}

void readTest() {
  int fd = open("test.bin", O_RDONLY);
  if (fd == -1) return;
  uint32_t id[2];
  size_t size = read(fd, (void*) id, 8);
  if (size == 8)
    printf("%d, %d\n", id[0], id[1]);
  close(fd);
}

typedef struct _curl_memory_t {
  uint8_t *memory;
  curl_off_t size;
  curl_off_t position;
} curl_mem_t;

void curlTest() {
  CURL *curl = curl_easy_init();
  if (curl == NULL) return;
  curl_mem_t apiTokenKey;
  memset(&apiTokenKey, 0, sizeof(curl_mem_t));
  void* cov = &apiTokenKey;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, cov);
  curl_easy_cleanup(curl);
  curl_global_cleanup();
}

int main(void) {
  readTest();
  curlTest();

  uuid_t uu;
  char usn[37];
  memset(usn, 0, sizeof(usn));
  uuid_unparse(uu, usn);
  strncpy(usn, "abcd", 4);

	FILE *fd = fopen("test.bin", "rb");
	int id[2];
	size_t size = fread((void*) id, sizeof(int), 2, fd);
	if (size == 2)
		printf("%d, %d\n", id[0], id[1]);
	fclose(fd);
	return EXIT_SUCCESS;
}

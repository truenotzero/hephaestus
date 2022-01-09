#ifndef FILE_H_
#define FILE_H_

struct hexfile;

struct hexfile *file_new();
struct hexfile *file_open(char const *const path);
int file_save(struct hexfile *const);
int file_save_as(struct hexfile *const, char const *const path);
int file_close(struct hexfile *);

#endif // FILE_H_
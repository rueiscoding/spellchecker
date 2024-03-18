#ifndef TRAVERSE_H
#define TRAVERSE_H

struct FileNode;

int is_valid_file(const char *filename);
int is_valid_directory(char *directory);
void add_file_to_list(const char *path);
void traverse_directory(const char *directory);
void free_file_list();
struct FileNode* command_line_traverse(const char *argument);
int matches_base_filename(const char *filename, const char *base_filename);

#endif
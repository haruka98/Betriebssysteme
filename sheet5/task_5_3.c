#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	char cwd[PATH_MAX];
	DIR* current_dir = opendir(getcwd(cwd, sizeof(cwd)));
	struct dirent* stc = readdir(current_dir);
	while(stc != NULL) {
		struct stat attr;
		stat(stc->d_name, &attr);
		printf("%s %ld\n", stc->d_name, attr.st_size);
		stc = readdir(current_dir);
	}
	closedir(current_dir);
	return 0;
}
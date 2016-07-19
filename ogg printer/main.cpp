#include <vorbis/vorbisfile.h>
#include <fstream>

class MyFile
{
private:
	std::fstream file;
public:
	static size_t read_cb(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
		f->file.read(reinterpret_cast<char *>(ptr), static_cast<std::streamsize>(size * nmemb));
		return static_cast<size_t>(f->file.gcount());
	}

	static int seek_cb(void *datasource, ogg_int64_t offset, int whence)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
		f->file.see
	}
};

typedef struct {
	size_t(*read_func)  (void *ptr, size_t size, size_t nmemb, void *datasource);
	int(*seek_func)  (void *datasource, ogg_int64_t offset, int whence);
	int(*close_func) (void *datasource);
	long(*tell_func)  (void *datasource);
} ov_callbacks;


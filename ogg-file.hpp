#include <cstdint>
#include <fstream>
struct OggVorbis_File;

namespace gearoenix
{
namespace frag
{
namespace audio
{
class OggFile
{
private:
	std::fstream file;
	static size_t read_cb (void *ptr, size_t size, size_t nmemb, void *datasource);
	static int    seek_cb (void *datasource, std::int64_t offset, int whence);
	static int    close_cb(void *datasource);
	static long   tell_cb (void *datasource);
	OggVorbis_File *oggvf;
public:
	OggFile(const std::string &file_name);
	std::uint64_t get_length();
};
}
}
}

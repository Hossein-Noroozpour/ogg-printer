#include <vorbis/vorbisfile.h>
#include <iostream>
#include "ogg-file.hpp"

size_t gearoenix::frag::audio::OggFile::read_cb(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	OggFile *f = reinterpret_cast<OggFile *>(datasource);
	f->file.read(reinterpret_cast<char *>(ptr), static_cast<std::streamsize>(size * nmemb));
	return static_cast<size_t>(f->file.gcount());
}

int gearoenix::frag::audio::OggFile::seek_cb(void *datasource, ogg_int64_t offset, int whence)
{
	OggFile *f = reinterpret_cast<OggFile *>(datasource);
	switch (whence)
	{
	case SEEK_SET:
		f->file.seekg(offset, std::ios_base::beg);
		break;
	case SEEK_CUR:
		f->file.seekg(offset, std::ios_base::cur);
		break;
	case SEEK_END:
		f->file.seekg(offset, std::ios_base::end);
		break;
	default:
		break;
	}
	return 0;
}

int gearoenix::frag::audio::OggFile::close_cb(void *datasource)
{
	OggFile *f = reinterpret_cast<OggFile *>(datasource);
	f->file.close();
	return 0;
}

long gearoenix::frag::audio::OggFile::tell_cb(void *datasource)
{
	OggFile *f = reinterpret_cast<OggFile *>(datasource);
	return static_cast<long>(f->file.tellg());
}

gearoenix::frag::audio::OggFile::OggFile(const std::string &file_name)
{
	static ov_callbacks ovcs =
	{
		(size_t (*)(void *, size_t, size_t, void *))read_cb,
		(int (*)(void *, ogg_int64_t, int))seek_cb,
		(int (*)(void *))close_cb,
		(long (*)(void *))tell_cb,
	};
	file.open(file_name, std::ios_base::binary | std::ios_base::in);
	if( 0 > ov_open_callbacks(this, oggvf, NULL, 0, ovcs))
	{
		std::cerr << "Openning ogg failed." << std::endl;
	}
	vorbis_info *vi = ov_info(oggvf, -1);
	std::cout << "Channels:        " << vi->channels << std::endl;
	std::cout << "Bitrate lower:   " << vi->bitrate_lower << std::endl;
	std::cout << "Bitrate nominal: " << vi->bitrate_nominal << std::endl;
	std::cout << "Bitrate upper:   " << vi->bitrate_upper << std::endl;
	std::cout << "Bitrate window:  " << vi->bitrate_window << std::endl;
	std::cout << "Codec setup:     " << vi->codec_setup << std::endl;
	std::cout << "Rate:            " << vi->rate << std::endl;
	std::cout << "Version:         " << vi->version << std::endl;

	while (true)
	{
		char data[8192];
		int useless = 0;
		long result = ov_read(oggvf, data, static_cast<int>(sizeof(data)), 0, 2, 1, &useless);
		switch(result)
		{
		case OV_HOLE:
			std::cerr << "indicates there was an interruption in the data. (one of: garbage between pages, loss of sync followed by recapture, or a corrupt page)" << std::endl;
			std::terminate();
		case OV_EBADLINK:
			std::cerr << "indicates that an invalid stream section was supplied to libvorbisfile, or the requested link is corrupt." << std::endl;
			std::terminate();
		case OV_EINVAL:
			std::cerr << "indicates the initial file headers couldn't be read or are corrupt, or that the initial open call for vf failed." << std::endl;
			std::terminate();
		}
		std::cout << "Result is " << result << std::endl;
		if(result == 0) break;
	}
}

std::uint64_t gearoenix::frag::audio::OggFile::get_length()
{
	return static_cast<std::uint64_t>(ov_pcm_total(oggvf, -1));
}

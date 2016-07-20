#include <vorbis/vorbisfile.h>
#include <fstream>
#include <iostream>
#include <xaudio2.h>
#include <Windows.h>
#include <wrl.h>

class MyFile
{
private:
	std::fstream file;

	static size_t read_cb(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
		f->file.read(reinterpret_cast<char *>(ptr), static_cast<std::streamsize>(size * nmemb));
		return static_cast<size_t>(f->file.gcount());
	}

	static int seek_cb(void *datasource, ogg_int64_t offset, int whence)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
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

	static int close_cb(void *datasource)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
		f->file.close();
		return 0;
	}

	static long tell_cb(void *datasource)
	{
		MyFile *f = reinterpret_cast<MyFile *>(datasource);
		return static_cast<long>(f->file.tellg());
	}

	ov_callbacks ovcs = 
	{
		(size_t(*)(void *, size_t, size_t, void *)) read_cb,
		(int(*)(void *, ogg_int64_t, int))          seek_cb,
		(int(*)(void *))                            close_cb,
		(long(*)(void *))                           tell_cb,
	};

	OggVorbis_File oggvf;

public:

	MyFile(const std::string &file_name)
	{
		file.open(file_name, std::ios_base::binary | std::ios_base::in);
		if( 0 > ov_open_callbacks(this, &oggvf, NULL, 0, ovcs))
			std::cerr << "Openning ogg failed." << std::endl;

		vorbis_info *vi = ov_info(&oggvf, -1);

		std::cout << "Channels:        " << vi->channels << std::endl;
		std::cout << "Bitrate lower:   " << vi->bitrate_lower << std::endl;
		std::cout << "Bitrate nominal: " << vi->bitrate_nominal << std::endl;
		std::cout << "Bitrate upper:   " << vi->bitrate_upper << std::endl;
		std::cout << "Bitrate window:  " << vi->bitrate_window << std::endl;
		std::cout << "Codec setup:     " << vi->codec_setup << std::endl;
		std::cout << "Rate:            " << vi->rate << std::endl;
		std::cout << "Version:         " << vi->version << std::endl;
	}

	std::uint64_t getLength()
	{
		return static_cast<std::uint64_t>(ov_pcm_total(&oggvf, -1));
	}


};

void main()
{
	MyFile f("C:\\Users\\Hossein\\Music\\1.ogg");
	Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		std::cerr << "Initialization error: " << hr << std::endl;
		std::terminate();
	}

	IXAudio2MasteringVoice* pMasterVoice = NULL;
	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
	{
		std::cerr << "rror: " << hr << std::endl;
		std::terminate();
	}

	WAVEFORMATEX wfx = { 0 };
	XAUDIO2_BUFFER buffer = { 0 };

	//buffer.AudioBytes = 

	int w;
	std::cin >> w;
}
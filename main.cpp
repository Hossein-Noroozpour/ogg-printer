#include <iostream>
#include "ogg-file.hpp"

int main()
{
#ifdef _WIN32
	gearoenix::frag::audio::OggFile f("C:\\Users\\hossein\\Dropbox\\Projects\\Start\\Music\\back-1.ogg");
#else
	gearoenix::frag::audio::OggFile f("/home/thany/Dropbox/Projects/Start/Music/back-1.ogg");
#endif
	int w;
	std::cin >> w;
	return 0;
}

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>

bool zlib_compress(boost::filesystem::path in_path, boost::filesystem::path out_path, boost::iostreams::zlib_params param = boost::iostreams::zlib::best_compression, int buffer_size = 65536)
{
	try
	{
		boost::filesystem::ifstream ifs(in_path, std::ios::in | std::ios::binary);
		std::string temp((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();

		boost::filesystem::fstream fs(out_path, std::ios::out | std::ios::binary);

		boost::iostreams::filtering_streambuf<boost::iostreams::output> out;
		out.push(boost::iostreams::zlib_compressor(param, buffer_size));
		out.push(fs);

		boost::iostreams::copy(boost::iostreams::basic_array_source<char>(&temp[0], temp.size()), out);
	}
	catch(std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: zlib_compress <file>" << std::endl;
		return 1;
	}

	std::string file(argv[1]);
	std::string compressed_file(file + ".z");

	if(zlib_compress(file, compressed_file))
	{
		uintmax_t uncompressed_size = boost::filesystem::file_size(file);
		uintmax_t compressed_size = boost::filesystem::file_size(compressed_file);

		std::cout << "Uncompressed: " << uncompressed_size << " B" << std::endl;
		std::cout << "Compressed: " << compressed_size << " B" << std::endl << std::endl;
		std::cin.get();
	}

	return 0;
}
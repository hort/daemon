#include "archive.hpp"

#include <cstring>

namespace hort
{

Archive::Archive() : is_open{false} {}

Archive::Archive(const std::string &filepath)
	: is_open{false}
{
	open(filepath);
};

Archive::~Archive()
{
	close();
}

bool Archive::open(const std::string &filepath)
{
	if (is_open)
		return false;
	archive = zip_open(filepath.c_str(), ZIP_CREATE, nullptr);
	return is_open = archive != nullptr;
}

void Archive::close()
{
	if (is_open)
		zip_close(archive);
	for (auto pointer : allocated)
		free(pointer);
	is_open = archive != nullptr;
}

int Archive::add(const std::string &filepath, const std::string &bin)
{
	if (is_open)
		return -2;
	zip_source *source = nullptr;
	int index = -1;

	// allocate the binary data in the heap, will later have free
	char *p = (char*)malloc(bin.size());
	allocated.emplace_back(p);
	memcpy(p, bin.c_str(), bin.size());

	// add a file entry in the archive and write the binary data to the file
	source = zip_source_buffer(archive, p, bin.size(), 1);
	index  = zip_file_add(archive, filepath.c_str(), source, ZIP_FL_OVERWRITE);

	return index;
}

int Archive::add(const std::map<std::string, std::string> &files)
{
	if (is_open)
		return -2;
	for (const auto &[f, b] : files)
		if (add(f, b) == -1)
			return -1;
	return 0;
}

} // namespace hort

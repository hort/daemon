#ifndef PASTEBIN_HPP_
#define PASTEBIN_HPP_

#include "hort/interface.hpp"

/// Forward Matches:
/// - https://pastebin.com/u/foo    -> download_user
/// - https://pastebin.com/2uDians0 -> download_paste
HORT_INTERFACE(Pastebin)
{

public:
	Pastebin(std::string username = "", std::string password = "");

	/// \brief Download all authors' pastes.
	int download_paste(std::string id);

	/// \brief Download paste given it's id, title and authors' username.
	/// \return true if paste is already downloaded
	int download_paste(std::string id, std::string title, std::string username);

	/// \brief Download paste given it's id.
	/// \return true if paste is already downloaded
	int download_user(std::string username);

private:

	/// parameters: paste
	const std::string url_rawpaste = "https://pastebin.com/raw/{}";

	/// parameters: user, page number
	const std::string url_user     = "https://pastebin.com/u/{}/{}";

	/// regex: \ <a href="/(\w*?)">(.*?)</a>
	const hort::re::Regex re_pastes;

};

#endif /* PASTEBIN_HPP_ */

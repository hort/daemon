#include "apis/pastebin.hpp"

Pastebin::Pastebin(std::string username, std::string password)
	: hort::Interface{"pastebin",
		{ { R"regex(^(?:https?://)?(?:www\.)?pastebin\.com/u/(\w+)$)regex", [this](const hort::re::Match &m) { download_user(m[0]);  } }
		, { R"regex(^(?:https?://)?(?:www\.)?pastebin\.com/(\w+)$)regex",   [this](const hort::re::Match &m) { download_paste(m[0]); } }
		}
	}
	, re_pastes{R"regex(\ <a href="/(\w*?)">(.*?)</a>)regex"} {}

int Pastebin::download_user(std::string username)
{
	int page       = 1;
	int downloaded = 0;
	while(true) {
		auto response = session.get(url_user, username, page++);
		auto pastes   = re_pastes.findall(response.body);
		if(pastes.size() == 0) break;
		for(int i = 0; i < pastes.size(); i += 2) {
			if(!download_paste(pastes[i], pastes[i + 1], username)) {
				downloaded++;
				sleep(1000);
			} else break;
		}
		sleep(2500);
	}
	return downloaded;
}

int Pastebin::download_paste(std::string id, std::string title, std::string username)
{
	hort::re::replace("/", "-", title);
	if(hort::filesystem::exists(hortpath + username + "/" + id + " - " + title)) return 1;
	session.download(hortpath + username, id + " - " + title, url_rawpaste, id);
	return 0;
}

int Pastebin::download_paste(std::string id)
{
	if(hort::filesystem::exists(hortpath + id)) return 1;
	session.download(hortpath, id, url_rawpaste, id);
	return 0;
}

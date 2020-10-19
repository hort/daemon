#include "interfaces/pastebin.hpp"

Pastebin::Pastebin()
  : hort::Interface{"pastebin",
    { { R"regex(^(?:https?://)?(?:www\.)?pastebin\.com/u/([\w\d_]+)$)regex",
        [this] (auto &v) {
          download_user(v[0]);
        } },
      { R"regex(^(?:https?://)?(?:www\.)?pastebin\.com/([\w\d]+)$)regex",
        [this] (auto &v) {
          download_paste(v[0]);
        } }
    } } {}

void Pastebin::download_user(const std::string &username) {
  auto page = 1;
  while(true) {
    auto response = session.get(url_user, username, page++);
    auto pastes = re_pastes.findall(response.body);
    if (pastes.size() == 0) {
      break;
    }
    for (auto i = 0u; i < pastes.size(); i += 2) {
      if (!download_paste(pastes[i], pastes[i + 1], username)) {
        hort::chrono::sleep(1000);
      } else {
        break;
      }
    }
    hort::chrono::sleep(2500);
  }
}

int Pastebin::download_paste(const std::string &id, std::string title, const std::string &username) {
  hort::re::replace("/", "-", title);
  if (hort::filesystem::exists(hortpath, username, id + " - " + title)) {
    return 1;
  }
  session.download(hortpath + username, id + " - " + title, url_rawpaste, id);
  return 0;
}

int Pastebin::download_paste(const std::string &id) {
  if (hort::filesystem::exists(hortpath, id)) {
    return 1;
  }
  session.download(hortpath, id, url_rawpaste, id);
  return 0;
}

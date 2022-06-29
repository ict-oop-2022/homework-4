#include "phone-book.h"

#include <cmath>
#include <algorithm>

static bool starts_with(const std::string& s, const std::string& pat)
{
  if (pat.size() > s.size()) {
    return false;
  }
  for (size_t i = 0; i < pat.size(); ++i) {
    if (s[i] != pat[i]) return false;
  }
  return true;
}

bool phone_book_t::create_user(const std::string &number, const std::string &name)
{
  auto [it, is_new] = user_info_by_number.insert({number, { {number, name}, 0 }});
  if (is_new) {
    user_info_for_name.insert(it->second);
    user_info_for_number[""].insert(it->second);
    std::string pref = "";
    for (auto ch: number) {
      pref += ch;
      user_info_for_number[pref].insert(it->second);
    }
  }
  return is_new;
}

bool phone_book_t::add_call(const call_t& call)
{
  auto it = user_info_by_number.find(call.number);
  if (it == user_info_by_number.end()) {
    return false;
  }

  user_info_for_name.erase(it->second);
  user_info_for_number[""].erase(it->second);
  std::string pref = "";
  for (auto ch: call.number) {
    pref += ch;
    user_info_for_number[pref].erase(it->second);
  }

  it->second.total_call_duration_s += call.duration_s;

  user_info_for_name.insert(it->second);
  user_info_for_number[""].insert(it->second);
  pref = "";
  for (auto ch: call.number) {
    pref += ch;
    user_info_for_number[pref].insert(it->second);
  }

  calls.push_back(call);

  return true;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const
{
  if (start_pos >= calls.size()) {
    return {};
  }
  count = std::min(count, calls.size() - start_pos);
  return {calls.begin() + start_pos, calls.begin() + start_pos + count};
}

std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string& number_prefix, size_t count) const
{
  auto it = user_info_for_number.find(number_prefix);
  if (it == user_info_for_number.end()) return {};
  std::vector<user_info_t> res;
  for (auto it2 = it->second.begin(); it2 != it->second.end() and count--; ++it2) {
    res.push_back(*it2);
  }

  return res;
}

std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) const
{
  auto it = user_info_for_name.lower_bound(
    {{"", name_prefix}, INFINITY}
  );
  std::vector<user_info_t> res;
  for (; it != user_info_for_name.end() and count; ++it, --count) {
    if (!starts_with(it->user.name, name_prefix)) {
      break;
    }
    res.push_back(*it);
  }
  return res;
}

void phone_book_t::clear() {
  user_info_by_number.clear();
  calls.clear();
  user_info_for_name.clear();
  user_info_for_number.clear();
}

bool phone_book_t::CompareUserInfoForName::operator()(const user_info_t& lhs, const user_info_t& rhs) const
{
  int name_compare = lhs.user.name.compare(rhs.user.name);
  if (name_compare == 0) {
    if (lhs.total_call_duration_s == rhs.total_call_duration_s) {
      return lhs.user.number < rhs.user.number;
    }
    return lhs.total_call_duration_s > rhs.total_call_duration_s;
  }
  return name_compare < 0;
}

bool phone_book_t::CompareUserInfoForNumber::operator()(const user_info_t& lhs, const user_info_t& rhs) const
{
  if (lhs.total_call_duration_s == rhs.total_call_duration_s) {
    int name_compare = lhs.user.name.compare(rhs.user.name);
    if (name_compare == 0) {
      return lhs.user.number < rhs.user.number;
    }
    return name_compare < 0;
  }
  return lhs.total_call_duration_s > rhs.total_call_duration_s;
}
// vim: bs=2 tabstop=2 softtabstop=2 shiftwidth=2 expandtab
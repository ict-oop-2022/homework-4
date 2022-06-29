#include "phone-book.h"
#include <cassert>
#include <cmath>
#include <limits>

bool phone_book_t::create_user(const string &number, const string &name) {
  auto it = contacts.find(number);
  if (it != contacts.end()) return false;
  SharedUserInfo userInfo = createSharedInfo(number, name);
  contacts[number] = userInfo;
  addToIndexes(userInfo);
  return true;
}

bool phone_book_t::add_call(const call_t &call) {
  auto it = contacts.find(call.number);
  if (it == contacts.end()) return false;
  if (call.duration_s != 0) {
    SharedUserInfo &userInfo = it->second;
    _removeFromIndexes(userInfo);
    userInfo->total_call_duration_s += call.duration_s;
    _addToIndexes(userInfo);
  }
  callHistory.push_back(call);
  return true;
}

vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const {
  vector <call_t> result;
  auto end_pos = min(_callHistory.size(), start_pos + count);
  if (end_pos > start_pos) {
    result.reserve(end_pos - start_pos);
    for (auto i = start_pos; i < end_pos; ++i) result.push_back(_callHistory[i]);
  }
  return result;
}

vector<user_info_t> phone_book_t::search_users_by_number(const string &number_prefix, size_t count) const {
  auto it = numberIndex.find(number_prefix);
  if (it == numberIndex.end()) return {};
  vector<user_info_t> result;
  for (const auto &sharedUserInfo : it->second) {
    if (count == 0) break;
    result.push_back(*sharedUserInfo);
    --count;
  }
  return result;
}

bool starts_with_prefix(const string &s, const string &prefix) {
  if (prefix.size() > s.size()) return false;
  for (size_t i = 0; i < prefix.size(); ++i)
    if (s[i] != prefix[i]) return false;
  return true;
}

vector<user_info_t> phone_book_t::search_users_by_name(const string &name_prefix, size_t count) const {
  SharedUserInfo dummy = createSharedInfo("", name_prefix, INFINITY);
  vector<user_info_t> result;
  auto it = nameIndex.lower_bound(dummy);
  while (count != 0 && it != nameIndex.end() && starts_with_prefix((*it)->user.name, name_prefix)) {
    result.push_back(**it);
    ++it;
    --count;
  }
  return result;
}

void phone_book_t::clear() {
  nameIndex.clear();
  numberIndex.clear();
  callHistory.clear();
  contacts.clear();
}

size_t phone_book_t::size() const { return contacts.size(); }

bool phone_book_t::empty() const { return contacts.empty(); }

SharedUserInfo phone_book_t::createSharedInfo(const string &number, const string &name, double duration) {
  return make_shared <user_info_t> (user_info_t{user_t{number, name}, duration});
}

void phone_book_t::addIndexes(const SharedUserInfo &userInfo) {
  if (userInfo.get() == nullptr) throw invalid_argument("Wrong input");
  auto it = nameIndex.insert(userInfo);
  if (!it.second) throw invalid_argument("Null Pointer Exception");
  addNumberIndex(userInfo);
}

void phone_book_t::removeIndexes(const SharedUserInfo &userInfo) {
  if (userInfo.get() == nullptr) throw invalid_argument("Wrong input");
  auto n = nameIndex.erase(userInfo);
  if (n == 0) throw invalid_argument("Null Pointer Exception");
  removeNumberIndex(userInfo);
}

void phone_book_t::addNumberIndex(const SharedUserInfo &userInfo) {
  if (userInfo.get() == nullptr) throw invalid_argument("Wrong input");
  PhoneNumberPrefix numberPrefix = "";
  auto it = numberIndex[numberPrefix].insert(userInfo);
  if (!it.second) throw invalid_argument("Null Pointer Exception");
  for (char i : userInfo->user.number) {
    numberPrefix += i;
    it = numberIndex[numberPrefix].insert(userInfo);
    if (!it.second) throw invalid_argument("Null Pointer Exception");
  }
}

void phone_book_t::removeNumberIndex(const SharedUserInfo &userInfo) {
  if (userInfo.get() == nullptr) throw invalid_argument("Wrong input");
  PhoneNumberPrefix numberPrefix = "";
  auto n = numberIndex[numberPrefix].erase(userInfo);
  if(n == 0) throw invalid_argument("Null Pointer Exception");
  for (char i : userInfo->user.number) {
    numberPrefix += i;
    n = numberIndex[numberPrefix].erase(userInfo);
    if(n == 0) throw invalid_argument("Null Pointer Exception");
  }
}

bool phone_book_t::NameIndexComparator operator()(const SharedUserInfo &a, const SharedUserInfo &b) const {
  if (a.get() == nullptr || b.get() == nullptr) throw invalid_argument("Invalid input");
  auto comp = a->user.name.compare(b->user.name);
  if (comp != 0) return comp < 0;
  else if (a->total_call_duration_s != b->total_call_duration_s)
    return a->total_call_duration_s > b->total_call_duration_s;
  else return a->user.number < b->user.number;
}

bool phone_book_t::NumberIndexComparator::operator()(const SharedUserInfo &a, const SharedUserInfo &b) const {
  if (a.get() == nullptr || b.get() == nullptr) throw invalid_argument("Invalid input");
  if (a->total_call_duration_s != b->total_call_duration_s) return a->total_call_duration_s > b->total_call_duration_s;
  else {
    auto comp = a->user.name.compare(b->user.name);
    if (comp != 0) return comp < 0;
    else return a->user.number < b->user.number;
  }
}
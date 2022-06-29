#include "phone-book.h"

bool phone_book_t::create_user(const std::string &number, const std::string &name) {
  for (int i=0; i < Contacts.size(); ++i) {
    if (Contacts[i].number == number){
      return false;
    }
  }
  user_t Contact;
  Contact.name = name;
  Contact.number = number;
  Contacts.push_back(Contact);
  user_info_t User_information;
  User_information.user = Contact;
  User_info.push_back(User_information);
  return true;
}

bool phone_book_t::add_call(const call_t &call) {
  for (int i=0; i < User_info.size(); ++i) {
    if (User_info[i].user.number == call.number) {
      Calls.push_back(call);
      User_info[i].total_call_duration_s += call.duration_s;
      return true;
    }
  }
  return false;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const{
    if (count == 0) {
      std::vector<call_t> call_history;
      return call_history;
    }
    if (start_pos > Calls.size()) {
      std::vector<call_t> call_history;
      return call_history;
    }
    if (start_pos+count > Calls.size()) {
      std::vector<call_t> call_history(Calls.begin()+start_pos, Calls.end());
      return call_history;
    }
  std::vector<call_t> call_history(Calls.begin()+start_pos, Calls.begin()+start_pos+count);
  return call_history;
}

std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) const{
  std::vector<user_info_t> User_info_copy(User_info.begin(), User_info.end());
  for (int i = 0; i < User_info_copy.size(); ++i){
    for (int ii = i+1; ii < User_info_copy.size(); ++ii){
      if (User_info_copy[i].user.name > User_info_copy[ii].user.name){
        auto biggest = User_info_copy[ii];
        User_info_copy[ii] = User_info_copy[i];
        User_info_copy[i] = biggest;
      }
      if (User_info_copy[i].user.name == User_info_copy[ii].user.name){
        if (User_info_copy[i].total_call_duration_s < User_info_copy[ii].total_call_duration_s){
          auto alph_order = User_info_copy[ii];
          User_info_copy[ii] = User_info_copy[i];
          User_info_copy[i] = alph_order;
        }
        if (User_info_copy[i].total_call_duration_s == User_info_copy[ii].total_call_duration_s){
          if (User_info_copy[i].user.number > User_info_copy[ii].user.number){
            auto alph_order1 = User_info_copy[ii];
            User_info_copy[ii] = User_info_copy[i];
            User_info_copy[i] = alph_order1;
          }
        }
      }
    }
  }
  if (count == 0){
    std::vector<user_info_t> result;
    return result;
  }
  if (name_prefix.empty()){
    if (count >= User_info_copy.size()){
      return User_info_copy;
    }
    else {
      std::vector<user_info_t> byname(User_info_copy.begin(), User_info_copy.begin()+count);
      return byname;
    }
  }
  std::vector<user_info_t> returned_name;
  for (int i =0; i < User_info_copy.size(); ++i) {
    if (User_info_copy[i].user.name.rfind(name_prefix, 0) == 0){
      returned_name.push_back(User_info_copy[i]);
    }
  }
  if (count >= returned_name.size()){
    return returned_name;
  }
  else {
    std::vector<user_info_t> re_name(returned_name.begin(), returned_name.begin()+count);
    return re_name;
  }
}


std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string &number_prefix, size_t count) const{
  std::vector<user_info_t> User_info_copy(User_info.begin(), User_info.end());
  for (int i = 0; i < User_info_copy.size(); ++i){
    for (int ii = i+1; ii < User_info_copy.size(); ++ii){
      if (User_info_copy[i].total_call_duration_s < User_info_copy[ii].total_call_duration_s){
        auto biggest = User_info_copy[ii];
        User_info_copy[ii] = User_info_copy[i];
        User_info_copy[i] = biggest;
      }
      if (User_info_copy[i].total_call_duration_s == User_info_copy[ii].total_call_duration_s){
        if (User_info_copy[i].user.name > User_info_copy[ii].user.name){
          auto alph_order = User_info_copy[ii];
          User_info_copy[ii] = User_info_copy[i];
          User_info_copy[i] = alph_order;
        }
        if (User_info_copy[i].user.name == User_info_copy[ii].user.name){
          if (User_info_copy[i].user.number > User_info_copy[ii].user.number){
            auto alph_order1 = User_info_copy[ii];
            User_info_copy[ii] = User_info_copy[i];
            User_info_copy[i] = alph_order1;
          }
        }
      }
    }
  }
  if (count == 0){
    std::vector<user_info_t> result;
    return result;
  }
  if (number_prefix.empty()){
    if (count >= User_info_copy.size()){
      return User_info_copy;
    }
    else {
      std::vector<user_info_t> bynumbers(User_info_copy.begin(), User_info_copy.begin()+count);
      return bynumbers;
    }
  }
  std::vector<user_info_t> returned_num;
  for (int i =0; i < User_info_copy.size(); ++i){
    if (User_info_copy[i].user.number.rfind(number_prefix, 0) == 0){
      returned_num.push_back(User_info_copy[i]);
    }
  }
  if (count >= returned_num.size()){
    return returned_num;
  }
  else {
    std::vector<user_info_t> re_num(returned_num.begin(), returned_num.begin()+count);
    return re_num;
  }
}

void phone_book_t::clear(){
  Contacts.clear();
  Calls.clear();
  User_info.clear();
}
size_t phone_book_t::size() const{
    return Contacts.size();
}

bool phone_book_t::empty() const{
  return (Contacts.empty() && Calls.empty() && User_info.empty());
}

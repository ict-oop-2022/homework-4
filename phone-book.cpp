#include "phone-book.h"



bool phone_book_t::create_user(const std::string &number, const std::string &name){
  for (auto & i : user_book){
    if (i.number == number){
      return false;
    }
  }
  user_book.push_back({number, name});
  user_info_book.push_back({user_t{number,name}, 0});
  return true;
}


bool phone_book_t::add_call(const call_t &call) {

  if (user_book.empty()){
    return false;
  }
    for (auto & i : user_book){
      if (i.number.compare(call.number) == 0){
        call_book.push_back({call.number, call.duration_s});
        for (auto & e : user_info_book){
          if (e.user.number == call.number){
            e.total_call_duration_s += call.duration_s;
          }
        }
        return true;
      }
    }
  return false;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const{
  std::vector<call_t> history;
  int otr = start_pos + count - 1;
  for (int i = start_pos; i <= otr; i++){
    if(i < call_book.size()){
      history.push_back({call_book[i].number,call_book[i].duration_s});
    }
  }
  return history;
}


std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) const {

  std::vector<user_info_t> more_prefix;
  std::vector<user_info_t> all_prefix;
  int len = name_prefix.length();

  if (name_prefix.length() == 0) {
    for (const auto &h : user_info_book) {
      more_prefix.push_back({h.user, h.total_call_duration_s});
    }
  }else {
    for (const auto & h: user_info_book){
      if (h.user.name.substr(0, len) == name_prefix) {
        more_prefix.push_back({h.user, h.total_call_duration_s});
      }
    }
  }


  for (const auto & c: more_prefix){
    for (int d = 0; d < more_prefix.size() -1; d++){
      if (more_prefix[d].user.name > more_prefix[d + 1].user.name) {
        user_info_t temp = more_prefix[d];
        more_prefix[d] = more_prefix[d + 1];
        more_prefix[d + 1] = temp;
      }
      else if (more_prefix[d].user.name == more_prefix[d + 1].user.name){
        if (more_prefix[d].total_call_duration_s < more_prefix[d + 1].total_call_duration_s) {
          user_info_t temp_tot = more_prefix[d];
          more_prefix[d] = more_prefix[d + 1];
          more_prefix[d + 1] = temp_tot;
        }
        else if (more_prefix[d].total_call_duration_s == more_prefix[d + 1].total_call_duration_s){
          if (more_prefix[d].user.number > more_prefix[d + 1].user.number) {
            user_info_t temp_num = more_prefix[d];
            more_prefix[d] = more_prefix[d + 1];
            more_prefix[d + 1] = temp_num;
          }
        }
      }

    }
  }



  for (int g = 0; g < count; g++){
    if (g < more_prefix.size()){
      all_prefix.push_back({more_prefix[g].user, more_prefix[g].total_call_duration_s});
    }
  }
  return all_prefix;
}


std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string &number_prefix, size_t count) const{
  std::vector<user_info_t> num_prefix;
  std::vector<user_info_t> all_num_prefix;
  int len = number_prefix.length();

  if (number_prefix.length() == 0) {
    for (const auto &h : user_info_book) {
      num_prefix.push_back({h.user, h.total_call_duration_s});
    }
  }else {
    for (const auto & h: user_info_book){
      if (h.user.number.substr(0, len) == number_prefix) {
        num_prefix.push_back({h.user, h.total_call_duration_s});
      }
    }
  }



  for (const auto & c: num_prefix){
    for (int d = 0; d < num_prefix.size() -1; d++){
      if (num_prefix[d].total_call_duration_s < num_prefix[d + 1].total_call_duration_s) {
        user_info_t temp = num_prefix[d];
        num_prefix[d] = num_prefix[d + 1];
        num_prefix[d + 1] = temp;
      }
      else if (num_prefix[d].total_call_duration_s == num_prefix[d + 1].total_call_duration_s){
        if (num_prefix[d].user.name > num_prefix[d + 1].user.name) {
          user_info_t temp_num = num_prefix[d];
          num_prefix[d] = num_prefix[d + 1];
          num_prefix[d + 1] = temp_num;
        }
        else if (num_prefix[d].user.name == num_prefix[d + 1].user.name){
          if (num_prefix[d].user.number > num_prefix[d + 1].user.number) {
            user_info_t temp_num = num_prefix[d];
            num_prefix[d] = num_prefix[d + 1];
            num_prefix[d + 1] = temp_num;
          }
        }
      }

    }
  }

  for (int g = 0; g < count; g++){
    if (g < num_prefix.size()){
      all_num_prefix.push_back({num_prefix[g].user, num_prefix[g].total_call_duration_s});
    }
  }
  return all_num_prefix;
}

void phone_book_t::clear(){
  call_book.clear();
  call_book.shrink_to_fit();
  user_book.clear();
  user_book.shrink_to_fit();
  user_info_book.clear();
  user_info_book.shrink_to_fit();
}



size_t phone_book_t::size() const{
    return user_book.size();
}



bool phone_book_t::empty() const{
    return user_book.empty();
}
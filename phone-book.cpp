#include "phone-book.h"


phone_book_t& phone_book_t::clear() {
    return (*this = phone_book_t());
}
bool phone_book_t::create_user(const std::string &number, const std::string &name) {
    if (users.count(number) == 1) {
        return false;
    }
    user_info_t info;
    info.user = {number,name};
    users[number] = info;
    book.insert(info);
    std::string prefix("");
    for (size_t i = 0; i <= number.size(); i ++){
        book_numbers[prefix].insert(info);

        if (i < number.size()){
            prefix += number[i];
        }
    }
    return true;

}

bool phone_book_t::add_call(const call_t &call) {
    if (users.count(call.number) == 0) {
        return false;
    }
    calls_to_num.push_back(call);
    std::string prefix("");
    user_info_t mem = users[call.number];
    for (int ind = 0; ind <=  call.number.size(); ind++ ){
        book_numbers.at(prefix).erase(mem);
        mem.total_call_duration_s += call.duration_s;
        book_numbers.at(prefix).insert(mem);
        if (ind == call.number.size()){
            users[call.number] = mem;
        }
        mem.total_call_duration_s -= call.duration_s;
        if (ind <  call.number.size()){
            prefix +=  call.number[ind];
        }
    }
    book.erase(mem);
    mem.total_call_duration_s += call.duration_s;
    book.insert(mem);
    return true;
}

std::vector<call_t> phone_book_t::get_calls(size_t start_pos, size_t count) const {
    if (start_pos > calls_to_num.size()){
        return std::vector<call_t>{};
    }else{
        size_t max_line = std::min(calls_to_num.size(),start_pos+count);
        return std::vector<call_t>(calls_to_num.begin() + start_pos, calls_to_num.begin()+max_line);
    }
}

std::vector<user_info_t> phone_book_t::search_users_by_number(const std::string &number_prefix, size_t count) {

    if (book_numbers.count(number_prefix) == 0 ){
        return std::vector<user_info_t>();
    }
    std::vector<user_info_t> answer;
    for (auto iter = book_numbers.at(number_prefix).begin(); iter!=book_numbers.at(number_prefix).end(); iter++){
        if (answer.size() >= count){
            break;
        }
        answer.push_back(*iter);
    }
    return answer;
}

std::vector<user_info_t> phone_book_t::search_users_by_name(const std::string &name_prefix, size_t count) {

    user_info_t check;
    std::vector<user_info_t> answer;
    check.user.name = name_prefix;
    check.total_call_duration_s = std::numeric_limits<double>::max();

    for (auto iter = book.lower_bound(check); iter != book.end(); iter++){
        if (count == 0 || (iter->user.name.substr(0,name_prefix.size()) != name_prefix && !name_prefix.empty())){
            break;
        }
        count--;
        answer.push_back(*iter);
    }
    return answer;
}

bool phone_book_t::empty() const {
    return size() == 0;
}

size_t phone_book_t::size() const {
    return book.size();
}
#include <string>
#include <iostream>

#include <sss/serializable.h>


struct Passport : sss::json_serializable<Passport>
{
    Passport() = default;
    Passport(std::string name, std::string surname)
        : name(std::move(name)), surname(std::move(surname))
    {}
    SSS_FIELD(std::string, name){"John"};
    SSS_FIELD(std::string, surname){"Smith"};
};

//struct Person : sss::json_serializable<Person>
//{
//    SSS_FIELD(Passport, passport);
//    SSS_FIELD(int, age) = 42;
//    SSS_FIELD(float, height) = 185.5;
//};
//
//struct Employee : sss::json_serializable<Employee>
//{
//    SSS_FIELD(Person, person);
//    SSS_FIELD(int, salary) {100500};
//    SSS_FIELD(int, vacationLeft) = 14;
//};

int main()
{
    std::make_from_tuple<Passport>(std::tuple(std::string{ "11" }, std::string{ "22" }));

    auto passport = Passport();
    passport.name = {"Lara"};
    passport.surname = {"Croft"};
    const auto text = passport.to_string();
    std::cout << text << std::endl;

    auto respored_passport = Passport();
    respored_passport.from_string(text);
    std::cout << respored_passport.to_string() << std::endl;
}

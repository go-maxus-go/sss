#include <string>
#include <iostream>

#include <sss/serializable.h>


struct Passport : sss::json_serializable<Passport>
{
    SSS_FIELD(std::string, name){"John"};
    SSS_FIELD(std::string, surname){"Smith"};
};

struct Person : sss::json_serializable<Person>
{
    SSS_FIELD(Passport, passport);
    SSS_FIELD(int, age) = 42;
    SSS_FIELD(float, height) = 185.5;
    SSS_FIELD(std::string, residence) {"Bearlin"};
    SSS_FIELD(std::vector<std::string>, friends);
};

int main()
{
    auto passport = Passport();
    passport.name = {"Lara"};
    passport.surname = {"Croft"};

    auto person = Person();
    person.passport = passport;
    person.age = 33;
    person.height = 6.9f;
    person.residence = {"Kemerovo"};
    person.friends->push_back("cat");
    person.friends->push_back("dog");

    const auto text = person.to_string();
    std::cout << text << std::endl;

    auto respored_person = Person();
    respored_person.from_string(text);
    std::cout << respored_person.to_string() << std::endl;
}

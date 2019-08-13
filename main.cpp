#include <string>
#include <iostream>

#include <sss/serializable.h>


struct Passport : sss::json_serializable<Passport>
{
    Passport() = default;
    Passport(sss::json_serializable<Passport>, std::string name, std::string surname)
        : name(std::move(name)), surname(std::move(surname))
    {}
    SSS_FIELD(std::string, name){"John"};
    SSS_FIELD(std::string, surname){"Smith"};
};

struct Person : sss::json_serializable<Person>
{
    Person() = default;
    Person(sss::json_serializable<Person>, Passport passport, int age, float height)
        : passport(std::move(passport)), age(age), height(height)
    {}
    SSS_FIELD(Passport, passport);
    SSS_FIELD(int, age) = 42;
    SSS_FIELD(float, height) = 185.5;
};

//struct Employee : sss::json_serializable<Employee>
//{
//    SSS_FIELD(Person, person);
//    SSS_FIELD(int, salary) {100500};
//    SSS_FIELD(int, vacationLeft) = 14;
//};

int main()
{
    auto passport = Passport();
    passport.name = {"Lara"};
    passport.surname = {"Croft"};

    auto person = Person();
    person.passport = passport;
    person.age = 33;
    person.height = 6.9;

    const auto text = person.to_string();
    std::cout << text << std::endl;

    auto respored_person = Person();
    respored_person.from_string(text);
    std::cout << respored_person.to_string() << std::endl;
}

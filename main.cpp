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
    SSS_FIELD(float, height) = 185.5; // Uncomment me later
};

struct Employee : sss::json_serializable<Employee>
{
    SSS_FIELD(Person, person);
    SSS_FIELD(int, salary) {100500};
    SSS_FIELD(int, vacationLeft) = 14; // Comment me later
};

int main()
{
    std::cout << Employee().to_string() << std::endl;
}

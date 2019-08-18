#include <string>
#include <iostream>

#include <sss/serializable.h>


struct Passport : sss::json_serializable<Passport>
{
    SSS_FIELD(std::string, name){"John"};
    SSS_FIELD(std::string, surname){"Smith"};
};

struct Level : sss::json_serializable<Level>
{
    SSS_FIELD(Passport, passport);
    SSS_FIELD(int, width);
    SSS_FIELD(int, height);
    SSS_FIELD(std::string, style){"Default"};
    SSS_FIELD(std::string, residence) {"Bearlin"};
    SSS_FIELD(std::vector<int>, balls);
};

int main()
{
    auto level_from_file = Level();
    level_from_file.passport->name = std::string("Lara");
    level_from_file.passport->surname = std::string("Croft");
    level_from_file.style = std::string("Metallic");
    level_from_file.width = 7;
    level_from_file.height = 6;
    level_from_file.balls->push_back(0);
    level_from_file.balls->push_back(0);
    level_from_file.balls->push_back(1);
    level_from_file.balls->push_back(0);
    level_from_file.balls->push_back(0);

    const auto text = level_from_file.to_string();
    std::cout << text << std::endl;

    auto level = Level();
    level.from_string(text);
    std::cout << level.to_string() << std::endl;
}

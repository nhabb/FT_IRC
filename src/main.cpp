#include "../includes/irc.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout<<"Invalid usage."<<std::endl;
        return (1);
    }
    if (validatePass(argv[2]))
        std::cout<<"Valid"<<std::endl;
    else
        return (1);
    return (0);
}

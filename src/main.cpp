#include "../includes/irc.hpp"


int main(int argc, char **argv)
{
    (void)argv;
    if (argc != 3)
    {
        std::cout<<"Invalid usage."<<std::endl;
        return (1);
    }
    std::cout<<"Welcome to IRC"<<std::endl;
    return (0);
}
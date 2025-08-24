#include "../includes/irc.hpp"


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout<<"Invalid usage."<<std::endl;
        return (1);
    }
    if (checkPort(argv[1]) && validatePass(argv[2]))
    {
        Server Server(argv[1],argv[2]);
    }
    else
    {
        if (!checkPort(argv[1]))
            std::cout<<"Invalid port"<<std::endl;
        else if (!validatePass(argv[2]))
            std::cout<<"Invalid password"<<std::endl;
        return (1);
    }
    return (0);
}

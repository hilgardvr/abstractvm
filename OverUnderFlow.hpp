#ifndef __OVERUNDER
# define __OVERUNDER

#include <iostream>

class OverUnderFlow : public std::exception {
    public:
        OverUnderFlow(std::string error);
        ~OverUnderFlow() throw();
        OverUnderFlow    &operator=(OverUnderFlow const &rhs);
        OverUnderFlow(OverUnderFlow const &src);
        virtual const char* what() const throw();

        std::string getError() const;

    private:
        OverUnderFlow();
        std::string _error;
};

#endif
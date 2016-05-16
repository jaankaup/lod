#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <stdexcept>

/**
 * Yksinkertainen luokka loggausta varten.
 *
 * Esim. Log myLogger("WARNING", std::cout);
 * myLogger.log("Tämä on %. loggaus. Loggaaminen on % kivaa. Tulostetaan && merkki.", 145, "MELKO");
 *
 * tulostus: WARNING: Tämä on 145. loggaus. Loggaaminen on MELKO kivaa. Tulostetaan & merkki.
 * 
 * %-merkki korvataan siis merkkijonon jälkeen tulevilla argumenteilla. Jos &-merkkien ja argumenttien määrä eivät vastaa, log funktio heittää poikkeuksen. 
 */

class Log
{
public:
    Log(const std::string& prefix, std::ostream& output) : prefix_(prefix), os_(output) {};

    void log(const char* str)
    {
        printPrefix();

        while (*str)
        {
            if (*str == '%')
            {
                if (*(str+1) == '%')
                {
                    ++str;
                }
                else
                {
                    throw std::runtime_error("Log() : invalid format string: missing arguments.");
                }
            }
            os_ << *str++;
        }
        printPrefix_ = true;
	printEOF_ = false;
	os_ << std::endl;
    }

    template <typename T, typename... Args>
    void log(const char* str, T val, Args... args)
    {
        printPrefix();

        while (*str)
        {
            if (*str == '%')
            {
                if (*(str+1) == '%')
                {
                    ++str;
                }
                else
                {
                    os_ << val;
                    ++str;
                    log(str, args...);
                    return;
                }
            }
            os_ << *str++;
        }
        printPrefix_ = true;
	if (printEOF_) 
	{
		os_ << std::endl;
	}
    }

private:
    std::string prefix_;
    std::ostream& os_;
    bool printPrefix_ = true;
    bool printEOF_ = true;

    void printPrefix()
    {
        if (printPrefix_)
        {
            os_ << prefix_;
            printPrefix_ = false;
        }
    }

};

#endif //LOG_H

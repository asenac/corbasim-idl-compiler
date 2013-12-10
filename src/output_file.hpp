#ifndef OUTPUT_FILE_HPP
#define OUTPUT_FILE_HPP

#include <string>
#include <fstream>

namespace corbasim
{
namespace idl
{

    struct output_file
    {
        enum { INDENT_SIZE = 4 };
        std::ofstream o;
        unsigned int indent_level;

        output_file(const std::string& file_name) :
            o(file_name.c_str()),
            indent_level(0)
        {}

        ~output_file()
        {
            o << std::endl;
        }

        template< typename T >
        output_file& operator<<(const T& t)
        {
            o << t;
            return *this;
        }

        template< typename T >
        output_file& operator%(const T& t)
        {
            o   << std::endl
                << std::string(indent_level * INDENT_SIZE, ' ')
                << t;
            return *this;
        }
    };

    struct nested_region
    {
        output_file& file;

        nested_region(output_file& file_) : file(file_)
        {
            ++file.indent_level;
        }

        ~nested_region()
        {
            --file.indent_level;
        }
    };

} // namesace idl
} // namespace corbasim

#endif /* OUTPUT_FILE_HPP */


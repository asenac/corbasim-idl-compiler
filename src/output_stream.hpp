#ifndef OUTPUT_FILE_HPP
#define OUTPUT_FILE_HPP

#include <string>
#include <ostream>

namespace corbasim
{
namespace idl
{

    struct output_stream
    {
        enum { INDENT_SIZE = 4 };
        std::ostream& o;
        unsigned int indent_level;

        output_stream(std::ostream& o_) :
            o(o_),
            indent_level(0)
        {}

        ~output_stream()
        {
            o << std::endl;
        }

        template< typename T >
        output_stream& operator<<(const T& t)
        {
            o << t;
            return *this;
        }

        template< typename T >
        output_stream& operator%(const T& t)
        {
            o   << std::endl
                << std::string(indent_level * INDENT_SIZE, ' ')
                << t;
            return *this;
        }
    };

    struct nested_region
    {
        output_stream& file;

        nested_region(output_stream& file_) : file(file_)
        {
            ++file.indent_level;
        }

        ~nested_region()
        {
            --file.indent_level;
        }

        template< typename T >
        nested_region& operator<<(const T& t)
        {
            file << t;
            return *this;
        }

        template< typename T >
        nested_region& operator%(const T& t)
        {
            file % t;
            return *this;
        }
    };

} // namesace idl
} // namespace corbasim

#endif /* OUTPUT_FILE_HPP */


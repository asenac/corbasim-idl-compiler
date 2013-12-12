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

        output_stream& operator++()
        {
            ++indent_level;
            return *this;
        }

        output_stream& operator--()
        {
            --indent_level;
            return *this;
        }

        output_stream& operator*()
        {
            o << std::endl;
            return *this;
        }
    };

    struct nested
    {
        output_stream& os;

        nested(output_stream& os_) : os(os_)
        {}

        template< typename T >
        nested& operator<<(const T& t)
        {
            os << t;
            return *this;
        }

        template< typename T >
        nested& operator%(const T& t)
        {
            os % t;
            return *this;
        }
    };


    struct nested_region : nested
    {
        nested_region(output_stream& os_) : nested(os_)
        {
            ++os;
        }

        ~nested_region()
        {
            --os;
        }
    };

    struct embrace : nested
    {
        embrace(output_stream& os_) : nested(os_)
        {
            os % '{';
            ++os;
        }

        ~embrace()
        {
            --os;
            os % '}';
        }
    };

    struct once : nested
    {
        const std::string s;
        once(output_stream& os_, const std::string& s_) : 
            nested(os_), s(s_)
        {
            os % "#ifndef " << s;
            os % "#define " << s;
            *os;
        }

        ~once()
        {
            os % "#endif // " << s;
        }
    };

    struct namespace_ : nested
    {
        const std::string name;
        namespace_(output_stream& os_, const std::string& name_) : 
            nested(os_), name(name_)
        {
            os % "namespace " << name;
            os % '{';
            *os;
            ++os;
        }

        ~namespace_()
        {
            --os;
            os % "} // namespace " << name;
            *os;
        }
    };

} // namesace idl
} // namespace corbasim

#endif /* OUTPUT_FILE_HPP */


#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <idlmm_forward.hpp>

namespace corbasim
{
namespace idl
{

    struct generator
    {
        bool generate(idlmm::TranslationUnit_ptr tu);
    };

} /* idl */ 
} /* corbasim */ 

#endif /* GENERATOR_HPP */

